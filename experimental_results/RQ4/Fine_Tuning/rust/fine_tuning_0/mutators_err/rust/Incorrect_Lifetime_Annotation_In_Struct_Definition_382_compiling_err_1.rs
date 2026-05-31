use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Incorrect_Lifetime_Annotation_In_Struct_Definition_382;

impl Mutator for Incorrect_Lifetime_Annotation_In_Struct_Definition_382 {
    fn name(&self) -> &str {
        "Incorrect_Lifetime_Annotation_In_Struct_Definition_382"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.first() {
                    if let syn::GenericParam::Lifetime(lifetime_param) = generics {
                        let original_lifetime = lifetime_param.lifetime.clone();
                        lifetime_param.lifetime = syn::Lifetime::new("'static", Span::call_site());

                        for field in &mut item_struct.fields {
                            if let syn::Type::Reference(type_reference) = &mut field.ty {
                                if let Some(lifetime) = &mut type_reference.lifetime {
                                    if *lifetime == original_lifetime {
                                        *lifetime = syn::Lifetime::new("'static", Span::call_site());
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
        "This mutation operator targets struct definitions with lifetime annotations and replaces them with the 'static lifetime. The mutation ensures that all references within the struct that use the original lifetime are updated to 'static, potentially causing lifetime mismatches and testing the compiler's lifetime management system."
    }
}