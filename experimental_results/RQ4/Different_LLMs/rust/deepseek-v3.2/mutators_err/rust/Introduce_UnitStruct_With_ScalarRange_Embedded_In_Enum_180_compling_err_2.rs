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

pub struct Introduce_UnitStruct_With_ScalarRange_Embedded_In_Enum_180;

impl Mutator for Introduce_UnitStruct_With_ScalarRange_Embedded_In_Enum_180 {
    fn name(&self) -> &str {
        "Introduce_UnitStruct_With_ScalarRange_Embedded_In_Enum_180"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut new_items = Vec::new();
        let mut insert_positions = Vec::new();
        let mut idx = 0;
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                if let Some(attr) = item_enum.attrs.iter().find(|a| a.path().is_ident("derive")) {
                    if let Ok(meta) = attr.parse_args::<syn::Meta>() {
                        if let syn::Meta::List(list) = meta {
                            if list.tokens.clone().into_iter().any(|token| {
                                let token_str = token.to_string();
                                token_str.contains("Default")
                            }) {
                                let has_default_variant = item_enum.variants.iter().any(|v| {
                                    v.attrs.iter().any(|a| a.path().is_ident("default"))
                                });
                                if has_default_variant {
                                    let existing_variant_names: HashSet<String> = item_enum.variants.iter().map(|v| v.ident.to_string()).collect();
                                    let new_struct_name = generate_unique_name(&existing_variant_names, "MutatedStruct");
                                    let new_variant_name = generate_unique_name(&existing_variant_names, "MutatedVariant");
                                    let scalar_range_attr = if rng.gen_bool(0.5) {
                                        format!("rustc_layout_scalar_valid_range_start({})", rng.gen_range(0..=5))
                                    } else {
                                        format!("rustc_layout_scalar_valid_range_end({})", rng.gen_range(0..=5))
                                    };
                                    let new_struct = parse_quote! {
                                        #[#scalar_range_attr]
                                        struct #new_struct_name;
                                    };
                                    insert_positions.push((idx, new_struct));
                                    let new_variant = parse_quote! {
                                        #new_variant_name(#new_struct_name),
                                    };
                                    item_enum.variants.push(new_variant);
                                }
                            }
                        }
                    }
                }
            }
            idx += 1;
        }
        for (pos, item) in insert_positions {
            file.items.insert(pos, item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets enums that derive Default and have a #[default] variant. It introduces a new unit-like struct annotated with rustc_layout_scalar_valid_range_start or rustc_layout_scalar_valid_range_end before the enum, and adds a new tuple variant containing that struct to the enum. This creates a layout computation scenario that may trigger compiler bugs related to scalar valid range attributes and enum layout."
    }
}

fn generate_unique_name(existing: &HashSet<String>, base: &str) -> Ident {
    let mut rng = thread_rng();
    let mut candidate = base.to_string();
    while existing.contains(&candidate) {
        candidate = format!("{}{}", base, rng.gen_range(0..1000));
    }
    Ident::new(&candidate, Span::call_site())
}