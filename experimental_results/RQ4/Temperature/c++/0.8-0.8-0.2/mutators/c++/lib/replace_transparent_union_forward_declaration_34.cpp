//source file
#include "../include/replace_transparent_union_forward_declaration_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("typedefDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (auto *UT = TD->getUnderlyingType()->getAs<clang::RecordType>()) {
        if (UT->getDecl()->isUnion() && UT->getDecl()->getAttr<clang::TransparentUnionAttr>()) {
          if (auto *RD = dyn_cast<clang::RecordDecl>(UT->getDecl()))
          {
            if (RD->isThisDeclarationADefinition() == clang::VarDecl::DeclarationOnly) {
              //Get the source code text of target node
              auto forwardDecl = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
              //Perform mutation on the source code text by applying string replacement
              std::string newDecl = "union " + RD->getNameAsString() + " { int member; };";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(RD->getSourceRange(), newDecl);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl(hasType(recordType(hasDeclaration(recordDecl(isUnion(), hasAttr(clang::attr::TransparentUnion)))))).bind("typedefDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}