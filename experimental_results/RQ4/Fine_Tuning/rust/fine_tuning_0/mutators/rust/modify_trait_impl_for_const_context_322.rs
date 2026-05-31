use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default::Default, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Modify_Trait_Impl_For_Const_Context_322;

impl Mutator for Modify_Trait_Impl_For_Const_Context_322 {
    fn name(&self) -> &str {
        "Modify_Trait_Impl_For_Const_Context_322"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                // Check if the trait is marked with #[const_trait]
                let const_trait_attr = item_impl.attrs.iter().position(|attr| {
                    attr.path().is_ident("const_trait")
                });

                if const_trait_attr.is_some() {
                    // Remove the #[const_trait] attribute
                    item_impl.attrs.remove(const_trait_attr.unwrap());
                }

                if let Some((_, path, _)) = &item_impl.trait_ {
                    // Modify the trait implementation to be a const trait implementation
                    let new_path = SynPath {
                        leading_colon: path.leading_colon,
                        segments: {
                            let mut segments = path.segments.clone();
                            if let Some(first_segment) = segments.first_mut() {
                                first_segment.ident = Ident::new(
                                    &format!("const_{}", first_segment.ident),
                                    first_segment.ident.span(),
                                );
                            }
                            segments
                        },
                    };
                    item_impl.trait_ = Some((None, new_path, item_impl.trait_.as_ref().unwrap().2.clone()));
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator modifies trait implementations to use const trait implementations. It removes the #[const_trait] attribute if present and adds the const keyword to the trait name in the impl block. This transformation tests the compiler's handling of const trait implementations, potentially revealing issues in const trait compatibility and enforcement."
    }
}