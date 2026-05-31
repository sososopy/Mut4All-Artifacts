//source file
#include "../include/Mutator_Replace_Member_Access_With_Arbitrary_Identifier_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MA = Result.Nodes.getNodeAs<clang::MemberExpr>("MemberExpr")) {
      //Filter nodes in header files
      if (!MA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MA->getBeginLoc()))
        return;
      
      // Check if this is a member access with dot operator
      if (!MA->isArrow()) {
        // Get the base expression (should be a namespace or class name)
        clang::Expr *Base = MA->getBase();
        if (!Base) return;
        
        // Get the member name
        clang::ValueDecl *MemberDecl = MA->getMemberDecl();
        if (!MemberDecl) return;
        
        std::string MemberName = MemberDecl->getNameAsString();
        if (MemberName.empty()) return;
        
        // Collect candidate identifiers from the translation unit if not already done
        if (candidateIdentifiers.empty()) {
          clang::TranslationUnitDecl *TU = Result.Context->getTranslationUnitDecl();
          for (clang::Decl *D : TU->decls()) {
            if (clang::NamedDecl *ND = llvm::dyn_cast<clang::NamedDecl>(D)) {
              if (ND->getIdentifier() && !ND->getNameAsString().empty()) {
                candidateIdentifiers.push_back(ND->getNameAsString());
              }
            }
          }
        }
        
        // If no candidates found, use a simple identifier
        std::string NewIdentifier = "x";
        if (!candidateIdentifiers.empty()) {
          size_t idx = getrandom::getRandomIndex(candidateIdentifiers.size() - 1);
          NewIdentifier = candidateIdentifiers[idx];
        }
        
        //Get the source code text of target node
        clang::SourceRange MemberRange = MA->getMemberLoc();
        if (MemberRange.isInvalid()) return;
        
        //Perform mutation on the source code text by applying string replacement
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(MemberRange, NewIdentifier);
      }
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = memberExpr().bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}