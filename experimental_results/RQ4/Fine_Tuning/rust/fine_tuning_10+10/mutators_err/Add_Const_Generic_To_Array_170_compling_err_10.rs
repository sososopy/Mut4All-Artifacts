use crate::mutator::Mutator;
use syn::{parse_quote, Item, FnArg, PatType, Type, Expr, Ident, GenericParam, TypePath, ExprPath, Path as SynPath, token};
use proc_macro2::Span;

pub struct Add_Const_Generic_To_Array_170;

impl Mutator for Add_Const_Generic_To_Array_170 {
    fn name(&self) -> &str {
        "Add_Const_Generic_To_Array_170"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut func) = item {
                let mut new_generics = func.sig.generics.clone();
                let mut modified = false;

                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(PatType { ty, .. }) = input {
                        if let Type::Reference(ref mut type_ref) = **ty {
                            if let Type::Array(ref mut type_array) = *type_ref.elem {
                                if let Expr::Path(ref expr_path) = *type_array.len {
                                    if let Some(ident) = expr_path.path.get_ident() {
                                        let generic_ident = Ident::new("N", ident.span());
                                        if !new_generics.params.iter().any(|param| {
                                            if let GenericParam::Const(ref const_param) = param {
                                                const_param.ident == generic_ident
                                            } else {
                                                false
                                            }
                                        }) {
                                            new_generics.params.push(GenericParam::Const(syn::ConstParam {
                                                attrs: Vec::new(),
                                                const_token: token::Const(Span::call_site()),
                                                ident: generic_ident.clone(),
                                                colon_token: token::Colon(Span::call_site()),
                                                ty: Type::Path(TypePath {
                                                    qself: None,
                                                    path: parse_quote!(usize),
                                                }),
                                                eq_token: None,
                                                default: None,
                                            }));
                                            type_array.len = Box::new(Expr::Path(ExprPath {
                                                attrs: Vec::new(),
                                                qself: None,
                                                path: SynPath::from(generic_ident.clone()),
                                            }));
                                            modified = true;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if modified {
                    func.sig.generics = new_generics;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function signatures, specifically those with parameters involving referenced slices or arrays. It introduces a generic constant expression by adding a const generic parameter placeholder. This increases the complexity of the type system in the function signature, which can expose potential compiler bugs related to type inference and const evaluation."
    }
}