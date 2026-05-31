use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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
use syn::PatReference;

pub struct Add_Nested_Impl_Iterator_With_Generic_Const_Array_392;

impl Mutator for Add_Nested_Impl_Iterator_With_Generic_Const_Array_392 {
    fn name(&self) -> &str {
        "Add_Nested_Impl_Iterator_With_Generic_Const_Array_392"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                let has_const_generics = impl_item.generics.params.iter().any(|param| matches!(param, syn::GenericParam::Const(_)));
                if !has_const_generics {
                    impl_item.generics.params.push(parse_quote!(const Z: usize));
                    for impl_item in &mut impl_item.items {
                        if let syn::ImplItem::Fn(func) = impl_item {
                            let generics = &mut func.sig.generics;
                            generics.params.push(parse_quote!('a));
                            generics.params.push(parse_quote!(U: 'a));
                            if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                                *return_type = Box::new(parse_quote!(impl Iterator<Item = impl Iterator<Item = [u8; Z]> + '_> + '_));
                            }
                            if let Some(syn::FnArg::Receiver(receiver)) = func.sig.inputs.get_mut(0) {
                                receiver.reference = Some(PatReference {
                                    and_token: Some(token::And {
                                        spans: [Span::call_site()],
                                    }),
                                    path: receiver.self_token.clone().into(),
                                });
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}