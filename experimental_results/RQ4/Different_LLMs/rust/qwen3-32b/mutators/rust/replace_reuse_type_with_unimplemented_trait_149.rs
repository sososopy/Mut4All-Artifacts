use rand::prelude::SliceRandom;
use syn::Expr;
use syn::visit_mut::VisitMut;
use syn::visit_mut;
use syn::parse_quote;
use rand::thread_rng;
use crate::mutator::Mutator;

struct MutatorVisitor<'a> {
    existing_types: &'a Vec<syn::Ident>,
    rng: rand::rngs::ThreadRng,
}

impl<'a> VisitMut for MutatorVisitor<'a> {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        visit_mut::visit_expr_mut(self, node);
        if let Expr::Path(expr_path) = node {
            if let Some(first_segment) = expr_path.path.segments.first() {
                if first_segment.ident == "reuse" && expr_path.qself.is_some() {
                    if let Some(new_type) = self.existing_types.choose(&mut self.rng) {
                        if let Some(qself) = &mut expr_path.qself {
                            qself.ty = Box::new(parse_quote! { #new_type });
                        }
                    }
                }
            }
        }
    }
}

pub struct Replace_Reuse_Type_With_Unimplemented_Trait_149;

impl Mutator for Replace_Reuse_Type_With_Unimplemented_Trait_149 {
    fn name(&self) -> &str {
        "Replace_Reuse_Type_With_Unimplemented_Trait_149"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut existing_types = Vec::new();
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                existing_types.push(s.ident.clone());
            }
            if let syn::Item::Enum(e) = item {
                existing_types.push(e.ident.clone());
            }
            if let syn::Item::Union(u) = item {
                existing_types.push(u.ident.clone());
            }
        }

        if existing_types.is_empty() {
            return;
        }

        let mut visitor = MutatorVisitor {
            existing_types: &existing_types,
            rng: thread_rng(),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies 'reuse' delegations and replaces the source type with an existing type that does not implement the required trait. This introduces invalid trait method calls, potentially exposing bugs in the compiler's handling of trait-bound method resolution and delegation logic."
    }
}