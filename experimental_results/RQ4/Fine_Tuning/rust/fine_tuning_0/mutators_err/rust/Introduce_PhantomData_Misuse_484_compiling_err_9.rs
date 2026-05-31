use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, spanned::Spanned, visit_mut::VisitMut, Expr, File, Item, ItemStruct, Stmt, Type,
    Fields, Field,
};

use crate::mutator::Mutator;

pub struct Introduce_PhantomData_Misuse_484;

impl Mutator for Introduce_PhantomData_Misuse_484 {
    fn name(&self) -> &str {
        "Introduce_PhantomData_Misuse_484"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut new_items = Vec::new();
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut has_phantom = false;
                for field in &item_struct.fields {
                    if let Type::Path(type_path) = &field.ty {
                        if type_path.path.segments.iter().any(|seg| seg.ident == "PhantomData") {
                            has_phantom = true;
                            break;
                        }
                    }
                }

                if !has_phantom {
                    let phantom_field: Field = parse_quote! {
                        phantom: std::marker::PhantomData<U>
                    };
                    if let Fields::Named(fields_named) = &mut item_struct.fields {
                        fields_named.named.push(phantom_field);
                    }

                    let struct_name = &item_struct.ident;
                    let init_expr: Expr = parse_quote! {
                        let instance = #struct_name {
                            phantom: std::marker::PhantomData,
                            ..Default::default()
                        };
                    };

                    let new_stmt = Stmt::Expr(init_expr);
                    new_items.push(Item::Verbatim(quote! { #new_stmt }));
                }
            }
        }
        file.items.extend(new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator introduces a PhantomData field with an undeclared generic type parameter U into struct definitions that do not already use PhantomData. It also ensures the struct is initialized with this PhantomData field, potentially causing type mismatches and testing the compiler's handling of type inference and phantom data usage."
    }
}