use syn::{Item, ItemConst, Local, Pat, PatIdent, Type, TypeReference, Expr, ExprReference, ExprPath, punctuated::Punctuated, PathSegment, PathArguments};
use proc_macro2::{Ident, Span};
use syn::token;

struct Replace_Const_With_Non_Const_Ref_328;

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}

impl Mutator for Replace_Const_With_Non_Const_Ref_328 {
    fn name(&self) -> &str {
        "Replace_Const_With_Non_Const_Ref_328"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Type::Reference(ref_type) = *const_item.ty {
                    let new_local = Local {
                        attrs: vec![],
                        let_token: token::Let {
                            span: Span::call_site(),
                        },
                        pat: Pat::Ident(PatIdent {
                            attrs: vec![],
                            by_ref: None,
                            mutability: None,
                            ident: Ident::new(&format!("{}_ref", const_item.ident), Span::call_site()),
                            subpat: None,
                        }),
                        init: Some(Expr::Reference(ExprReference {
                            attrs: vec![],
                            and_token: token::And {
                                spans: [Span::call_site()],
                            },
                            mutability: ref_type.mutability,
                            expr: Box::new(Expr::Path(ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new(&format!("{}_ref", const_item.ident), Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            })),
                        })),
                        semi_token: const_item.semi_token,
                    };
                    file.items.push(Item::Const(syn::ItemConst {
                        attrs: const_item.attrs.clone(),
                        vis: const_item.vis.clone(),
                        const_token: const_item.const_token.clone(),
                        ident: Ident::new(&format!("{}_ref", const_item.ident), Span::call_site()),
                        colon_token: token::Colon { spans: [Span::call_site()] }, 
                        eq_token: const_item.eq_token.clone(),
                        ty: Box::new(Type::Reference(TypeReference {
                            lifetime: None,
                            and_token: token::And {
                                spans: [Span::call_site()],
                            },
                            mutability: ref_type.mutability,
                            elem: Box::new(Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new(&format!("{}_ref", const_item.ident), Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            })),
                        })),
                        expr: Box::new(Expr::Path(ExprPath {
                            attrs: vec![],
                            qself: None,
                            path: syn::Path {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new(&format!("{}_ref", const_item.ident), Span::call_site()),
                                    arguments: PathArguments::None,
                                }]),
                            },
                        })),
                        semi_token: const_item.semi_token.clone(),
                        generics: syn::Generics::default(),
                    }));
                    *item = Item::Const(syn::ItemConst {
                        attrs: const_item.attrs.clone(),
                        vis: const_item.vis.clone(),
                        const_token: const_item.const_token.clone(),
                        ident: const_item.ident.clone(),
                        colon_token: token::Colon { spans: [Span::call_site()] }, 
                        eq_token: const_item.eq_token.clone(),
                        ty: Box::new(Type::Reference(TypeReference {
                            lifetime: None,
                            and_token: token::And {
                                spans: [Span::call_site()],
                            },
                            mutability: ref_type.mutability,
                            elem: Box::new(Type::Path(syn::TypePath {
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new(&format!("{}_ref", const_item.ident), Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            })),
                        })),
                        expr: Box::new(Expr::Reference(ExprReference {
                            attrs: vec![],
                            and_token: token::And {
                                spans: [Span::call_site()],
                            },
                            mutability: ref_type.mutability,
                            expr: Box::new(Expr::Path(ExprPath {
                                attrs: vec![],
                                qself: None,
                                path: syn::Path {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new(&format!("{}_ref", const_item.ident), Span::call_site()),
                                        arguments: PathArguments::None,
                                    }]),
                                },
                            })),
                        })),
                        semi_token: const_item.semi_token.clone(),
                        generics: syn::Generics::default(),
                    });
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces constant references with non-constant references, potentially triggering bugs related to constant trait implementations."
    }
}