//source file
#include "../include/Mutator_102.h"

// ========================================================================================================
#define MUT102_OUTPUT 1

void MutatorFrontendAction_102::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SW = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!SW || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SW->getBeginLoc()))
        return;

      // Check if switch is inside constexpr or consteval function
      const auto* parentFunc = Result.Context->getParents(*SW)[0].get<clang::FunctionDecl>();
      if (!parentFunc) return;
      if (!parentFunc->isConstexpr() && !parentFunc->isConsteval()) return;

      // Collect all identifiers in current scope
      auto& SM = *Result.SourceManager;
      auto& LO = Result.Context->getLangOpts();
      std::set<std::string> localIds;
      
      // Function to collect identifiers from a declaration
      std::function<void(const clang::Decl*)> collectIds = [&](const clang::Decl* D) {
        if (auto* ND = llvm::dyn_cast<clang::NamedDecl>(D)) {
          if (auto* II = ND->getIdentifier()) {
            localIds.insert(II->getName().str());
          }
        }
        // Recursively check child declarations
        if (auto* DC = llvm::dyn_cast<clang::DeclContext>(D)) {
          for (auto* Child : DC->decls()) {
            collectIds(Child);
          }
        }
      };
      
      // Collect from parent function scope
      collectIds(parentFunc);
      
      // Generate unique undeclared identifier
      std::string newIdent;
      int counter = 0;
      do {
        newIdent = "undeclared_ident_" + std::to_string(counter++);
      } while (localIds.count(newIdent) > 0);
      
      // Get condition source range
      auto cond = SW->getCond();
      if (!cond) return;
      
      auto condRange = cond->getSourceRange();
      if (condRange.isInvalid()) return;
      
      // Perform mutation on the source code text by applying string replacement
      std::string replacement = newIdent;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(condRange, replacement);
    }
}
  
void MutatorFrontendAction_102::MutatorASTConsumer_102::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = switchStmt().bind("SwitchStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}