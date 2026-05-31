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

pub struct Insert_Union_With_Packed_Enum_184;

impl Mutator for Insert_Union_With_Packed_Enum_184 {
    fn name(&self) -> &str {
        "Insert_Union_With_Packed_Enum_184"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut enum_definitions = Vec::new();
        for item in &file.items {
            if let Item::Enum(item_enum) = item {
                if let Some(repr) = extract_repr(&item_enum.attrs) {
                    enum_definitions.push((item_enum.ident.clone(), repr, item_enum.variants.len()));
                }
            }
        }
        if enum_definitions.is_empty() {
            return;
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    continue;
                }
                let selected = enum_definitions.choose(&mut rng).unwrap();
                let enum_name = &selected.0;
                let repr_type = selected.1;
                let variant_count = selected.2;
                let invalid_discriminant = variant_count + rng.gen_range(100..1000);
                let union_name = Ident::new(&format!("MutatedUnion_{}", enum_name), Span::call_site());
                let union_def: Item = parse_quote! {
                    union #union_name {
                        int: #repr_type,
                        #enum_name: #enum_name,
                    }
                };
                let union_init_stmt: Stmt = parse_quote! {
                    let _mu = #union_name { int: #invalid_discriminant };
                };
                item_fn.block.stmts.insert(0, union_init_stmt);
                item_fn.block.stmts.insert(0, Stmt::Item(union_def));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator searches for enums with explicit integer representation (e.g., #[repr(u32)]). It then selects a non-main function and inserts a union definition containing two fields: an integer matching the enum's repr and the enum itself. The union is instantiated with an integer value that likely does not correspond to a valid enum discriminant (e.g., exceeding variant count). This mutation tests compiler handling of packed enum layout within unions, especially when the stored integer may be invalid for the enum type, potentially triggering layout computation errors or discriminant validation issues."
    }
}

fn extract_repr(attrs: &[Attribute]) -> Option<Type> {
    for attr in attrs {
        if attr.path().is_ident("repr") {
            if let Ok(Meta::List(list)) = attr.parse_meta() {
                for nested in list.nested {
                    if let NestedMeta::Meta(Meta::Path(path)) = nested {
                        if let Some(ident) = path.get_ident() {
                            let repr_str = ident.to_string();
                            match repr_str.as_str() {
                                "u8" => return Some(parse_quote!(u8)),
                                "u16" => return Some(parse_quote!(u16)),
                                "u32" => return Some(parse_quote!(u32)),
                                "u64" => return Some(parse_quote!(u64)),
                                "usize" => return Some(parse_quote!(usize)),
                                "i8" => return Some(parse_quote!(i8)),
                                "i16" => return Some(parse_quote!(i16)),
                                "i32" => return Some(parse_quote!(i32)),
                                "i64" => return Some(parse_quote!(i64)),
                                "isize" => return Some(parse_quote!(isize)),
                                _ => continue,
                            }
                        }
                    }
                }
            }
        }
    }
    None
}