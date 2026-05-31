use syn::{FnArg, Type, TypePath, PathArguments, GenericArgument};
use crate::mutator::Mutator;

pub struct ReplaceGenericFunctionWithNonGenericFunction;

impl Mutator for ReplaceGenericFunctionWithNonGenericFunction {
    fn name(&self) -> &str {
        "ReplaceGenericFunctionWithNonGenericFunction"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if !func.sig.generics.params.is_empty() {
                    let generic_params = &func.sig.generics.params;
                    let first_param = &func.sig.inputs.first().unwrap();
                    if let FnArg::Typed(pat_type) = first_param {
                        let ty = &*pat_type.ty;
                        if let Type::Path(TypePath {
                            qself: None,
                            path: syn::Path { segments, .. },
                        }) = ty
                        {
                            if let Some(segment) = segments.first() {
                                if segment.ident == "T" {
                                    let new_ty = match segment.arguments {
                                        PathArguments::AngleBracketed(ref args) => {
                                            args.args.first().unwrap().clone()
                                        }
                                        _ => unreachable!(),
                                    };
                                    func.sig.generics.params.clear();
                                    if let FnArg::Typed(pat_type) = &mut *func.sig.inputs.first_mut().unwrap() {
                                        if let GenericArgument::Type(ty) = new_ty {
                                            *pat_type.ty = ty;
                                        } else {
                                            unreachable!();
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets generic functions in the given Rust code and replaces them with non-generic functions. It applies to functions that have generic parameters and are used as arguments to higher-order functions. This transformation aims to test the compiler's handling of type parameters and bindings in different contexts."
    }
}

fn test_main() {
    let mutator = ReplaceGenericFunctionWithNonGenericFunction;
    let mut file_ast = syn::parse_file("fn test<T>(x: T) {}").unwrap();
    mutator.mutate(&mut file_ast);
}