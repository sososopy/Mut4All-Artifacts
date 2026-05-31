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

pub struct Add_Impl_With_NonExistent_AssocType_391;

impl Mutator for Add_Impl_With_NonExistent_AssocType_391 {
    fn name(&self) -> &str {
        "Add_Impl_With_NonExistent_AssocType_391"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(struct_item) = item {
                let has_const_params = struct_item.generics.params.iter().any(|param| matches!(param, GenericParam::Const(_)));
                if !has_const_params {
                    continue;
                }

                let new_trait = parse_quote! {
                    pub trait Concat {}
                };
                file.items.push(syn::Item::Trait(Box::new(new_trait)));

                let struct_generics = &struct_item.generics;
                let mut impl_generics = struct_generics.clone();
                impl_generics.params.push(parse_quote!(J));

                let path_to_len = Path {
                    leading_colon: None,
                    segments: {
                        let mut segments = Punctuated::new();
                        let concat_segment = PathSegment {
                            ident: Ident::new("Concat", Span::call_site()),
                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                colon2_token: None,
                                lt_token: token::Lt::default(),
                                args: {
                                    let mut args = Punctuated::new();
                                    if let Some(GenericParam::Type(type_param)) = struct_generics.params.first() {
                                        args.push(GenericArgument::Type(Type::Path(TypePath {
                                            qself: None,
                                            path: Path {
                                                leading_colon: None,
                                                segments: {
                                                    let mut segs = Punctuated::new();
                                                    segs.push(PathSegment {
                                                        ident: type_param.ident.clone(),
                                                        arguments: PathArguments::None,
                                                    });
                                                    segs
                                                },
                                            },
                                        })));
                                    }
                                    args
                                },
                                gt_token: token::Gt::default(),
                            }),
                        };
                        segments.push(concat_segment);

                        let non_existent_segment = PathSegment {
                            ident: Ident::new("NonExistent", Span::call_site()),
                            arguments: PathArguments::None,
                        };
                        segments.push(non_existent_segment);

                        let field_segment = PathSegment {
                            ident: Ident::new("Field", Span::call_site()),
                            arguments: PathArguments::None,
                        };
                        segments.push(field_segment);

                        segments
                    },
                };

                let array_ty = TypeArray {
                    bracket_token: token::Bracket::default(),
                    elem: Box::new(Type::Path(TypePath {
                        qself: None,
                        path: parse_quote!(u8),
                    })),
                    len: Box::new(Type::Path(TypePath {
                        qself: None,
                        path: path_to_len,
                    })),
                };

                let mut predicates = Punctuated::new();
                let predicate = WherePredicate::Type(PredicateType {
                    lifetimes: None,
                    bounded_ty: Box::new(array_ty),
                    colon_token: token::Colon::default(),
                    bounds: {
                        let mut bounds = Punctuated::new();
                        bounds.push(parse_quote!(Sized));
                        bounds
                    },
                });
                predicates.push(predicate);

                let where_clause = WhereClause {
                    where_token: token::Where::default(),
                    predicates,
                };

                let trait_path = parse_quote!(Mul);
                let self_ty = parse_quote!(Tensor<I, N>);

                let impl_block = ItemImpl {
                    attrs: vec![],
                    defaultness: None,
                    unsafety: None,
                    impl_token: token::Impl::default(),
                    generics: impl_generics,
                    trait_: Some((trait_path, token::Colon2::default(), self_ty)),
                    self_ty: Box::new(self_ty),
                    where_clause: Some(where_clause),
                    items: vec![],
                };

                file.items.push(syn::Item::Impl(Box::new(impl_block)));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}