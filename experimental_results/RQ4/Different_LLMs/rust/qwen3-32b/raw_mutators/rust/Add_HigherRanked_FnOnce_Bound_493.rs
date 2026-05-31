use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Add_HigherRanked_FnOnce_Bound_493;

impl Mutator for Add_HigherRanked_FnOnce_Bound_493 {
    fn name(&self) -> &str {
        "Add_HigherRanked_FnOnce_Bound_493"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                let mut closure_param = None;
                let mut t_type = None;
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::ImplTrait(type_impl_trait) = &**pat_type.ty {
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.is_ident("Fn") {
                                        if let PathArguments::AngleBracketed(args) = &trait_bound.path.segments[0].arguments {
                                            if args.args.len() == 1 {
                                                if let GenericArgument::Type(arg_type) = &args.args[0] {
                                                    if let Type::Reference(ref_type) = &**arg_type {
                                                        if ref_type.mutability.is_some() {
                                                            closure_param = Some(pat_type);
                                                            t_type = Some(ref_type.elem.clone());
                                                            break;
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
                }

                if let (Some(pat_type), Some(t_type)) = (closure_param, t_type) {
                    if func.sig.asyncness.is_none() {
                        func.sig.asyncness = Some(token::Async {
                            span: Span::call_site(),
                        });
                    }

                    let f_ident = Ident::new("F", Span::call_site());
                    let f_param = GenericParam::Type(syn::TypeParam {
                        attrs: Vec::new(),
                        ident: f_ident.clone(),
                        colon_token: Some(Default::default()),
                        bounds: {
                            let mut bounds = Punctuated::new();
                            let fn_trait = TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath::from(Ident::new("Fn", Span::call_site())),
                            };
                            let args = Punctuated::from_iter(vec![GenericArgument::Type(Type::Reference(Box::new(TypeReference {
                                and_token: token::And { span: Span::call_site() },
                                lifetime: None,
                                mutability: Some(token::Mut { span: Span::call_site() }),
                                elem: Box::new(t_type.clone()),
                            }))]);
                            let path_args = PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                colon2_token: None,
                                lt_token: token::Lt { span: Span::call_site() },
                                args,
                                gt_token: token::Gt { span: Span::call_site() },
                            });
                            let path = SynPath {
                                leading_colon: None,
                                segments: Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("Fn", Span::call_site()),
                                    arguments: path_args,
                                }]),
                            };
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path,
                            }));
                            bounds
                        },
                        eq_token: None,
                        default: None,
                    });
                    func.sig.generics.params.push(f_param);

                    *pat_type.ty = Box::new(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(f_ident),
                    }));

                    let for_predicate = WherePredicate::For {
                        for_token: token::For { span: Span::call_site() },
                        lifetimes: BoundLifetimes {
                            lifetimes: vec![LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: Lifetime::new("'a", Span::call_site()),
                            }].into_iter().collect(),
                        },
                        predicate: Box::new(WherePredicate::Type(PredicateType {
                            lifetimes: None,
                            bounded_ty: Type::Path(TypePath {
                                qself: None,
                                path: SynPath::from(Ident::new("F", Span::call_site())),
                            }),
                            colon_token: token::Colon { span: Span::call_site() },
                            bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath {
                                    leading_colon: None,
                                    segments: Punctuated::from_iter(vec![PathSegment {
                                        ident: Ident::new("FnOnce", Span::call_site()),
                                        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                            colon2_token: None,
                                            lt_token: token::Lt { span: Span::call_site() },
                                            args: Punctuated::from_iter(vec![GenericArgument::Type(Type::Tuple(TypeTuple {
                                                paren_token: Paren { span: Span::call_site() },
                                                elems: Punctuated::from_iter(vec![Type::Reference(Box::new(TypeReference {
                                                    and_token: token::And { span: Span::call_site() },
                                                    lifetime: Some(Lifetime::new("'a", Span::call_site())),
                                                    mutability: Some(token::Mut { span: Span::call_site() }),
                                                    elem: Box::new(t_type.clone()),
                                                }))]),
                                            }))),
                                            gt_token: token::Gt { span: Span::call_site() },
                                        }),
                                    }],
                                },
                            })]),
                        })),
                    };

                    let plus_predicate = WherePredicate::Type(PredicateType {
                        lifetimes: None,
                        bounded_ty: Type::Path(TypePath {
                            qself: None,
                            path: SynPath::from(Ident::new("F", Span::call_site())),
                        }),
                        colon_token: token::Colon { span: Span::call_site() },
                        bounds: Punctuated::from_iter(vec![TypeParamBound::Lifetime(Lifetime::new("'a", Span::call_site()))]),
                    });

                    let mut predicates = Punctuated::new();
                    predicates.push(for_predicate);
                    predicates.push(plus_predicate);

                    if let Some(where_clause) = &mut func.sig.generics.where_clause {
                        where_clause.predicates.extend(predicates);
                    } else {
                        func.sig.generics.where_clause = Some(WhereClause {
                            where_token: token::Where { span: Span::call_site() },
                            predicates,
                        });
                    }

                    let mut new_block = func.block.clone();
                    new_block.stmts.push(Stmt::Local(Local {
                        attrs: Vec::new(),
                        let_token: token::Let { span: Span::call_site() },
                        pat: Box::new(Pat::Ident(PatIdent {
                            attrs: Vec::new(),
                            ident: Ident::new("_x", Span::call_site()),
                            subpat: None,
                            mutability: None,
                        })),
                        init: Some((token::Eq { span: Span::call_site() }, Box::new(Expr::Verbatim(quote! { unimplemented!() })))),
                        semi_token: Some(token::Semi { span: Span::call_site() }),
                        decl_token: None,
                    }));
                    new_block.stmts.push(Stmt::Expr(Expr::Reference(ExprReference {
                        and_token: token::And { span: Span::call_site() },
                        mutability: Some(token::Mut { span: Span::call_site() }),
                        expr: Box::new(Expr::Path(ExprPath {
                            attrs: Vec::new(),
                            qself: None,
                            path: SynPath::from(Ident::new("_x", Span::call_site())),
                        })),
                    }), None));
                    func.block = new_block;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}