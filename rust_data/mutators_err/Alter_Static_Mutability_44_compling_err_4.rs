use syn::{Item, Expr, StaticMutability, token::Mut};

pub struct Alter_Static_Mutability_44;

impl Mutator for Alter_Static_Mutability_44 {
    fn name(&self) -> &str {
        "Alter_Static_Mutability_44"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                let is_external_type = item_static.ty.to_token_stream().to_string().contains("Wrapper");
                let involves_unsafe = match &*item_static.expr {
                    Expr::Unsafe(_) => true,
                    _ => false,
                };

                if is_external_type || involves_unsafe {
                    item_static.mutability = match item_static.mutability {
                        StaticMutability::Mut(_) => StaticMutability::None,
                        StaticMutability::None => StaticMutability::Mut(Mut::default()),
                        _ => item_static.mutability.clone(),
                    };
                }
            }
        }
    }
    
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets static variable declarations that involve external types or unsafe operations. By altering the mutability of such static variables, we explore the impact on Rust's safety guarantees and synchronization requirements. This can reveal issues related to const evaluation and internal consistency checks, especially in the context of unsafe code."
    }
}