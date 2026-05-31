use syn::{parse_quote, Item, GenericParam, FnArg, Type, TypePath, TypeInfer, Path, PathSegment};
use crate::mutator::Mutator;

struct Replace_Const_Param_With_Inferred_Type_290;

impl Mutator for Replace_Const_Param_With_Inferred_Type_290 {
    fn name(&self) -> &str {
        "Replace_Const_Param_With_Inferred_Type_290"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Some(param) = struct_item.generics.params.iter().next() {
                    if let GenericParam::Const(param) = param {
                        if let Some(default_value) = &param.default {
                            let new_param = GenericParam::Const(syn::ConstParam {
                                attrs: param.attrs.clone(),
                                const_token: param.const_token,
                                ident: param.ident.clone(),
                                colon_token: param.colon_token,
                                ty: None,
                                default: Some(default_value.clone()),
                                eq_token: param.eq_token,
                            });
                            struct_item.generics.params[0] = new_param;
                        }
                    }
                }
            }
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(TypePath {
                            qself: None,
                            path: Path {
                                leading_colon: None,
                                segments,
                            },
                        }) = &*pat_type.ty
                        {
                            if let Some(segment) = segments.iter().next() {
                                if segment.ident == "const" {
                                    pat_type.ty = Box::new(Type::Infer(TypeInfer {
                                        underscore_token: Default::default(),
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the explicit type of a constant parameter with an inferred type, effectively removing the explicit type annotation. This can help expose bugs in the Rust compiler related to type inference and constant parameters."
    }
}