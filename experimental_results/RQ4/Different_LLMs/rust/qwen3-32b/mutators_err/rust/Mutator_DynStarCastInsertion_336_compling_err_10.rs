use crate::mutator::Mutator;
use syn::FnArg;
use syn::Type;
use syn::Pat;

pub struct Mutator_DynStarCastInsertion_336;

impl Mutator for Mutator_DynStarCastInsertion_336 {
    fn name(&self) -> &str {
        "Mutator_DynStarCastInsertion_336"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let block = &mut *func.block;
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let param_ty = &*pat_type.ty;
                        let is_unsized = match param_ty {
                            Type::TraitObject(_) => true,
                            Type::Path(type_path) => {
                                if let Some(segment) = type_path.path.segments.last() {
                                    segment.ident == "str" && type_path.qself.is_none()
                                } else {
                                    false
                                }
                            },
                            _ => false,
                        };
                        if is_unsized {
                            if let Pat::Ident(ident_pat) = &*pat_type.pat {
                                let param_name = ident_pat.ident.to_string();
                                let ptr_type = match param_ty {
                                    Type::TraitObject(trait_obj) => {
                                        Type::Ptr(syn::TypePtr {
                                            star_token: Default::default(),
                                            const_token: Some(Default::default()),
                                            mutability: None,
                                            elem: Box::new(Type::TraitObject(trait_obj.clone())),
                                        })
                                    },
                                    Type::Path(type_path) => {
                                        Type::Ptr(syn::TypePtr {
                                            star_token: Default::default(),
                                            const_token: Some(Default::default()),
                                            mutability: None,
                                            elem: Box::new(Type::Path(type_path.clone())),
                                        })
                                    },
                                    _ => unreachable!(),
                                };
                                let stmt = syn::parse_quote! { { let _: #ptr_type = unsafe { std::mem::transmute(#param_name) }; } };
                                block.stmts.insert(0, stmt);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a transmutation of unsized function parameters to raw pointer types (e.g., *const dyn Trait). This targets the compiler's handling of pointer conversions and unsized type coercions, potentially exposing ICEs when operand validation fails for raw pointer values derived from trait objects."
    }
}