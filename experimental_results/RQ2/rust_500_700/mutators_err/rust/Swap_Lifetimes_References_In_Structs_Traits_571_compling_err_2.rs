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
    ItemTrait, TraitItem, TraitItemMethod,
};

use crate::mutator::Mutator;

pub struct Swap_Lifetimes_References_In_Structs_Traits_571;

impl Mutator for Swap_Lifetimes_References_In_Structs_Traits_571 {
    fn name(&self) -> &str {
        "Swap_Lifetimes_References_In_Structs_Traits_571"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if let Some(generics) = &mut item_struct.generics.params.first_mut() {
                    if let GenericParam::Lifetime(lifetime_param) = generics {
                        let new_lifetime = Lifetime::new("'b", Span::call_site());
                        lifetime_param.lifetime = new_lifetime.clone();
                        for field in &mut item_struct.fields {
                            if let Type::Reference(type_ref) = &mut field.ty {
                                if let Some(lifetime) = &mut type_ref.lifetime {
                                    *lifetime = new_lifetime.clone();
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Trait(item_trait) = item {
                for item in &mut item_trait.items {
                    if let TraitItem::Method(method) = item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Path(type_path) = &mut *pat_type.ty {
                                    if let Some(segment) = type_path.path.segments.first_mut() {
                                        if segment.ident == "Container" {
                                            if let PathArguments::AngleBracketed(angle_args) = &mut segment.arguments {
                                                if let Some(GenericArgument::Lifetime(lifetime)) = angle_args.args.first_mut() {
                                                    *lifetime = Lifetime::new("'b", Span::call_site());
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
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets structs and trait method signatures involving lifetimes and references. It introduces a new lifetime parameter and swaps existing references to use this new lifetime. This transformation is intended to test the compiler's handling of lifetimes in terms of variance and region constraints, potentially revealing issues in lifetime resolution."
    }
}