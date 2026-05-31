//source file
#include "../include/modify_transparent_union_with_forward_declaration_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TransparentUnion")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (const auto *UT = TD->getUnderlyingType()->getAs<clang::RecordType>()) {
        if (const auto *UD = dyn_cast<clang::RecordDecl>(UT->getDecl())) {
          if (UD->hasAttr<clang::TransparentUnionAttr>()) {
            //Perform mutation on the source code text by applying string replacement
            std::string newUnionText = "typedef union { struct ForwardDecl; } " + TD->getNameAsString() + " __attribute__((__transparent_union__));";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(TD->getSourceRange(), newUnionText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl(hasType(recordType(hasDeclaration(recordDecl(hasAttr(attr::TransparentUnion)))))).bind("TransparentUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}