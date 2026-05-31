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

pub struct Redundant_Module_Reexports_623;

impl Mutator for Redundant_Module_Reexports_623 {
    fn name(&self) -> &str {
        "Redundant_Module_Reexports_623"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut reexported_modules = HashSet::new();
        for item in &mut file.items {
            if let Item::Use(item_use) = item {
                let path = &item_use.tree;
                if let UseTree::Path(use_path) = path {
                    let module_name = &use_path.ident;
                    if reexported_modules.contains(module_name) {
                        continue;
                    }
                    reexported_modules.insert(module_name.clone());
                    if let Some(last_segment) = use_path.path.segments.last() {
                        let reexport_path: UseTree = parse_quote! {
                            #module_name::#last_segment
                        };
                        let new_item: Item = Item::Use(ItemUse {
                            attrs: vec![],
                            vis: Visibility::Public(VisPublic {
                                pub_token: token::Pub {
                                    span: Span::call_site(),
                                },
                            }),
                            use_token: token::Use {
                                span: Span::call_site(),
                            },
                            leading_colon: None,
                            tree: reexport_path,
                            semi_token: token::Semi {
                                spans: [Span::call_site()],
                            },
                        });
                        file.items.push(new_item);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies modules that are re-exported using `pub use` statements. It introduces additional re-exports for these modules, potentially causing redundancy or cyclic re-exports. This transformation tests the Rust compiler's handling of multiple re-exports, particularly in the context of module visibility and `rustdoc` generation."
    }
}