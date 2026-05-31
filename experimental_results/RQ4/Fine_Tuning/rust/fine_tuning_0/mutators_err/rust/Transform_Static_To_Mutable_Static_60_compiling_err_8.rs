use syn::parse_quote;
use proc_macro2::{Span, Ident};
use syn::{Expr, ExprPath, visit_mut::VisitMut};
use crate::mutator::Mutator;

pub struct Transform_Static_To_Mutable_Static_60;

impl Mutator for Transform_Static_To_Mutable_Static_60 {
    fn name(&self) -> &str {
        "Transform_Static_To_Mutable_Static_60"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut unsafe_wrappers = Vec::new();
        
        for item in &mut file.items {
            if let syn::Item::Static(item_static) = item {
                if matches!(item_static.mutability, syn::StaticMutability::None) {
                    item_static.mutability = syn::StaticMutability::Mut(syn::token::Mut { span: Span::call_site() });
                    
                    // Collect identifiers for later unsafe wrapping
                    let var_ident = item_static.ident.clone();
                    unsafe_wrappers.push(UnsafeWrapper { var_ident });
                }
            }
        }
        
        for wrapper in unsafe_wrappers {
            wrapper.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms immutable static variables into mutable ones by changing `static` to `static mut`. It then wraps usages of these variables in unsafe blocks to ensure semantic correctness. This transformation tests the compiler's handling of mutable static variables and their interaction with unsafe code, potentially uncovering issues in constant evaluation and safety checks."
    }
}

struct UnsafeWrapper {
    var_ident: Ident,
}

impl VisitMut for UnsafeWrapper {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Path(ExprPath { path, .. }) = expr {
            if path.is_ident(&self.var_ident) {
                *expr = parse_quote! {
                    unsafe { #expr }
                };
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}