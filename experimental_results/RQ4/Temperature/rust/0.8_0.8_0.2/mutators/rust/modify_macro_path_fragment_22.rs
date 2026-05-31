use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, *,
};

use crate::mutator::Mutator;

pub struct Modify_Macro_Path_Fragment_22;

impl Mutator for Modify_Macro_Path_Fragment_22 {
    fn name(&self) -> &str {
        "Modify_Macro_Path_Fragment_22"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Macro(mac) = item {
                let mut rng = thread_rng();
                if let Some(ref mut mac_path) = mac.mac.path.segments.last_mut() {
                    let original = mac_path.ident.to_string();
                    let alternatives = vec![
                        "Debug",
                        "std_fmt_Debug",
                        "serde_Serialize",
                        "Option_i32",
                    ];

                    if let Some(new_path) = alternatives.choose(&mut rng) {
                        mac_path.ident = Ident::new(new_path, Span::call_site());
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies macro path fragments within macro invocations, replacing them with alternative valid paths to test the compiler's handling of macro expansion and path resolution. This involves substituting paths with other valid paths, nested paths, paths from external crates, or paths incorporating generics to explore robustness in handling complex path configurations."
    }
}