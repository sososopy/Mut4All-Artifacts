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

pub struct Introduce_PhantomData_Misuse_In_Struct_484;

impl Mutator for Introduce_PhantomData_Misuse_In_Struct_484 {
    fn name(&self) -> &str {
        "Introduce_PhantomData_Misuse_In_Struct_484"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_phantom_data = false;
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                for field in &item_struct.fields {
                    if let Type::Path(type_path) = &field.ty {
                        if type_path
                            .path
                            .segments
                            .last()
                            .map_or(false, |seg| seg.ident == "PhantomData")
                        {
                            has_phantom_data = true;
                            break;
                        }
                    }
                }
                if has_phantom_data {
                    break;
                }
            }
        }
        if has_phantom_data {
            return;
        }
        let mut rng = thread_rng();
        let mut target_structs = vec![];
        for (index, item) in file.items.iter().enumerate() {
            if let Item::Struct(item_struct) = item {
                target_structs.push(index);
            }
        }
        if target_structs.is_empty() {
            return;
        }
        let target_index = target_structs.choose(&mut rng).unwrap();
        if let Item::Struct(item_struct) = &mut file.items[*target_index] {
            let phantom_field: syn::Field = parse_quote! {
                phantom: PhantomData<U>
            };
            item_struct.fields.push(phantom_field);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets a random struct in the seed program that does not already use PhantomData and adds a PhantomData<U> field, where U is an undeclared generic type. This transformation exploits the compiler's handling of undeclared generics and PhantomData, potentially leading to type resolution errors and ICEs, especially in complex type inference scenarios."
    }
}