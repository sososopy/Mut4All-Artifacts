use crate::mutator::Mutator;
use proc_macro2::{Ident, Span};
use syn::{self, Item, ImplItem, Type, TypeArray, TypePath, Path, Expr, ExprCall, ExprPath, GenericParam, Generics, parse_quote};

pub struct Replace_AssocType_With_GenericConst_Min_114;

impl Mutator for Replace_AssocType_With_GenericConst_Min_114 {
    fn name(&self) -> &str {
        "Replace_AssocType_With_GenericConst_Min_114"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                let impl_generics = &impl_item.generics;
                let const_params: Vec<_> = impl_generics
                    .params
                    .iter()
                    .filter_map(|param| {
                        if let GenericParam::Const(const_param) = param {
                            Some(const_param.clone())
                        } else {
                            None
                        }
                    })
                    .collect();
                if const_params.len() < 2 {
                    continue;
                }
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Type(assoc_type) = impl_item {
                        if let Type::Path(_) = &*assoc_type.ty {
                            let c1 = &const_params[0].ident;
                            let c2 = &const_params[1].ident;
                            let min_call = ExprCall {
                                attrs: Vec::new(),
                                func: Box::new(Expr::Path(ExprPath {
                                    attrs: Vec::new(),
                                    qself: None,
                                    path: Path::from(Ident::new("min", Span::call_site())),
                                })),
                                paren_token: Default::default(),
                                args: {
                                    let mut args = syn::punctuated::Punctuated::new();
                                    args.push(Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: Path::from(c1.clone()),
                                    }));
                                    args.push(Expr::Path(ExprPath {
                                        attrs: Vec::new(),
                                        qself: None,
                                        path: Path::from(c2.clone()),
                                    }));
                                    args
                                },
                            };
                            let t_type = Type::Path(TypePath {
                                qself: None,
                                path: Path::from(Ident::new("T", Span::call_site())),
                            });
                            let array_type = TypeArray {
                                bracket_token: Default::default(),
                                elem: Box::new(t_type),
                                len: Box::new(Expr::Call(min_call)),
                                semi_token: Default::default(),
                            };
                            assoc_type.ty = Box::new(Type::Array(array_type));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces an associated type in a trait implementation with a generic const expression using an existing const function on the struct's const parameters. It leverages an existing min-like function to compute the const expression, transforming the associated type into an array with a length determined by the minimum of two const parameters. This tests the compiler's handling of const expressions in trait associated types and dynamic const evaluation in generic contexts."
    }
}