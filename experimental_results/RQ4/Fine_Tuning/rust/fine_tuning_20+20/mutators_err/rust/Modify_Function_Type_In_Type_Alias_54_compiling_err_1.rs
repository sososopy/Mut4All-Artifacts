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

pub struct Modify_Function_Type_In_Type_Alias_54;

impl Mutator for Modify_Function_Type_In_Type_Alias_54 {
    fn name(&self) -> &str {
        "Modify_Function_Type_In_Type_Alias_54"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Type(item_type) = item {
                if let Type::TraitObject(type_trait_object) = &*item_type.ty {
                    for bound in &type_trait_object.bounds {
                        if let TypeParamBound::Trait(trait_bound) = bound {
                            if trait_bound.path.is_ident("Fn")
                                || trait_bound.path.is_ident("FnMut")
                                || trait_bound.path.is_ident("FnOnce")
                            {
                                let new_trait_bound: TraitBound = parse_quote!(FnOnce(u32) -> u16);
                                item_type.ty = Box::new(Type::TraitObject(TypeTraitObject {
                                    dyn_token: type_trait_object.dyn_token,
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(new_trait_bound));
                                        bounds
                                    },
                                }));
                            }
                        }
                    }
                }
            }
        }
        struct TypeAliasCollector {
            type_aliases: Vec<Ident>,
        }
        impl<'ast> Visit<'ast> for TypeAliasCollector {
            fn visit_item_type(&mut self, node: &'ast ItemType) {
                self.type_aliases.push(node.ident.clone());
            }
        }
        let mut collector = TypeAliasCollector {
            type_aliases: Vec::new(),
        };
        collector.visit_file(file);
        let type_aliases = collector.type_aliases;
        struct TypeAliasReplacer {
            type_aliases: Vec<Ident>,
        }
        impl VisitMut for TypeAliasReplacer {
            fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
                if let Expr::Path(ExprPath { path, .. }) = &*node.func {
                    if self.type_aliases.iter().any(|alias| path.is_ident(alias)) {
                        node.args.push(parse_quote!(0u32));
                    }
                }
                syn::visit_mut::visit_expr_call_mut(self, node);
            }
            fn visit_type_path_mut(&mut self, node: &mut TypePath) {
                if self
                    .type_aliases
                    .iter()
                    .any(|alias| node.path.is_ident(alias))
                {
                    node.path = parse_quote!(u16);
                }
                syn::visit_mut::visit_type_path_mut(self, node);
            }
        }
        let mut replacer = TypeAliasReplacer { type_aliases };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets type aliases that define function types using the Fn, FnMut, or FnOnce traits. It modifies the function signature within the type alias by adding a u32 parameter and changing the return type to u16. It then updates all usages of the type alias in the code, ensuring that function calls receive the correct number of arguments and that any type paths referring to the alias are replaced with the new return type. This transformation stresses the compiler's handling of function traits and type alias resolution, especially in contexts involving const evaluation and type inference."
    }
}