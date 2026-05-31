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

pub struct Alter_Enum_Variants_45;

impl Mutator for Alter_Enum_Variants_45 {
    fn name(&self) -> &str {
        "Alter_Enum_Variants_45"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                for variant in &mut item_enum.variants {
                    if let syn::Fields::Unnamed(fields) = &mut variant.fields {
                        let mut new_fields = Vec::new();
                        for field in &mut fields.unnamed {
                            if let Type::Path(type_path) = &mut field.ty {
                                if let Some(segment) = type_path.path.segments.last_mut() {
                                    match segment.ident.to_string().as_str() {
                                        "u128" => {
                                            segment.ident = Ident::new("u64", Span::call_site());
                                            new_fields.push(parse_quote!(usize));
                                        }
                                        "u64" => {
                                            segment.ident = Ident::new("u128", Span::call_site());
                                            new_fields.push(parse_quote!(u8));
                                        }
                                        _ => {
                                            if type_path.path.is_ident("Void") {
                                                field.ty = parse_quote!(u128);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        fields.unnamed.extend(new_fields);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator focuses on altering enum variants by changing types of fields and modifying their number. It targets fields with uninhabited types or varying sizes, such as `u128` and `u64`, to challenge data layout assumptions. This transformation aims to provoke compiler issues related to transmutability and layout computation, particularly in enums with complex data layouts."
    }
}