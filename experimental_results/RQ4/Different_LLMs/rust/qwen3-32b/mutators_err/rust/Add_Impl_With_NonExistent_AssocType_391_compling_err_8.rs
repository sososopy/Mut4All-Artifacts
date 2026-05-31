use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeArray, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Colon},
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
                file.items.push(syn::Item::Trait(new_trait));

                let struct_generics = &struct_item.generics;
                let mut impl_generics = struct_generics.clone();
                impl_generics.params.push(parse_quote!(J));

                let path_to_len = parse_quote!(Concat::<T>::NonExistent::Field);

                let array_ty = TypeArray {
                    bracket_token: token::Bracket::default(),
                    elem: Box::new(Type::Path(TypePath {
                        qself: None,
                        path: parse_quote!(u8),
                    })),
                    len: Expr::Path(ExprPath {
                        attrs: vec![],
                        qself: None,
                        path: path_to_len,
                    }),
                    semi_token: token::Semi::default(),
                };

                let mut predicates = Punctuated::new();
                let predicate = WherePredicate::Type(PredicateType {
                    lifetimes: None,
                    bounded_ty: Type::Array(array_ty),
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
                impl_generics.where_clause = Some(where_clause);

                let trait_path: syn::Path = parse_quote!(Mul);
                let trait_self_ty = parse_quote!(Tensor<I, N>);
                let impl_type = parse_quote!(Tensor<I, N>);

                let colon_token = token::Colon::default();

                let impl_block = ItemImpl {
                    attrs: vec![],
                    defaultness: None,
                    unsafety: None,
                    impl_token: token::Impl::default(),
                    generics: impl_generics,
                    trait_: Some((trait_path, colon_token, trait_self_ty)),
                    self_ty: Box::new(impl_type),
                    items: vec![],
                    brace_token: token::Brace::default(),
                };

                file.items.push(syn::Item::Impl(impl_block));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}