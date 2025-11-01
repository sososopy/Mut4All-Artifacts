//source file
#include "../include/Modify_Const_Volatile_Qualifiers_OpenMP_347.h"

// ========================================================================================================
#define MUT347_OUTPUT 1

void MutatorFrontendAction_347::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      //Get the source code text of target node
      auto qualType = VD->getType();
      if (qualType.isConstQualified() || qualType.isVolatileQualified()) {
        //Perform mutation on the source code text by applying string replacement
        auto unqualifiedType = qualType.getUnqualifiedType();
        std::string newDecl = unqualifiedType.getAsString() + " " + VD->getNameAsString();
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(VD->getSourceRange(), newDecl);
      }
    }
}
  
void MutatorFrontendAction_347::MutatorASTConsumer_347::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasAncestor(ompExecutableDirective().bind("ompAtomic"))).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}