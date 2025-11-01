//source file
#include "../include/remove_static_const_template_member_166.h"

// ========================================================================================================
#define MUT166_OUTPUT 1

void MutatorFrontendAction_166::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("StaticConstMember")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      //Perform mutation on the source code text by applying string replacement
      SourceLocation startLoc = VD->getBeginLoc();
      SourceLocation endLoc = VD->getEndLoc().getLocWithOffset(1);
      Rewrite.RemoveText(SourceRange(startLoc, endLoc));
    }
}
  
void MutatorFrontendAction_166::MutatorASTConsumer_166::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasType(isConstQualified()), isStaticStorageClass(), hasParent(classTemplateDecl())).bind("StaticConstMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}