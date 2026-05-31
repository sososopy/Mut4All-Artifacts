use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Static_Structure_Field_Reordering_5;

impl Mutator for Static_Structure_Field_Reordering_5 {
    fn name(&self) -> &str {
        "Static_Structure_Field_Reordering_5"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.fields.iter().count() > 1 {
                    let new_order: Vec<_> = item_struct.fields.iter().cloned().collect();
                    let mut shuffled_order = new_order.clone();
                    shuffled_order.shuffle(&mut rng);
                    
                    let original_order: Vec<_> = item_struct.fields.iter().map(|f| f.ident.clone()).collect();
                    let field_map: std::collections::HashMap<_, _> = shuffled_order
                        .iter()
                        .zip(original_order.iter())
                        .map(|(new, old)| (old.clone(), new.clone()))
                        .collect();
                    
                    item_struct.fields = Fields::Named(FieldsNamed {
                        brace_token: item_struct.fields.span().into(),
                        named: Punctuated::from_iter(shuffled_order.into_iter()),
                    });
                    
                    for item in &mut file.items {
                        if let Item::Static(item_static) = item {
                            if let Expr::Struct(expr_struct) = &mut *item_static.expr {
                                expr_struct.fields = expr_struct
                                    .fields
                                    .iter()
                                    .map(|f| FieldValue {
                                        member: Member::Named(field_map[&f.member].ident.unwrap()),
                                        ..f.clone()
                                    })
                                    .collect();
                            }
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