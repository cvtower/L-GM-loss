#include <cfloat>
#include <vector>

#include "caffe/layers/mean_layer.hpp"
#include "caffe/util/math_functions.hpp"

namespace caffe {

template <typename Dtype>
void MeanLayer<Dtype>::Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  vector<int> shape = bottom[0]->shape();
  shape[0] /= this->layer_param_.reduction_param().axis();
  top[0]->Reshape(shape);
}

template <typename Dtype>
void MeanLayer<Dtype>::Forward_cpu(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  const Dtype *data = bottom[0]->cpu_data();
  Dtype *top_data = top[0]->mutable_cpu_data();
  int dim = top[0]->count(1);
  int K = this->layer_param_.reduction_param().axis();
  Dtype scale = Dtype(1) / K;
  for (int i = 0; i < top[0]->shape(0); ++i) {
    //caffe_copy(dim, data, top_data);
    caffe_cpu_axpby(dim, scale, data, Dtype(0), top_data);
    data += dim;
    for (int j = 1; j < K; ++j, data += dim)
        caffe_axpy(dim, scale, data, top_data);
    top_data += dim;
  }
}

#ifdef CPU_ONLY
STUB_GPU(MeanLayer);
#endif

INSTANTIATE_CLASS(MeanLayer);
REGISTER_LAYER_CLASS(Mean);

}  // namespace caffe
