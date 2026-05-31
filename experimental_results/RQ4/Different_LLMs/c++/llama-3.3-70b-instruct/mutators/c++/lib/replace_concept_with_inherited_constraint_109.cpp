//source file
#include "../include/Replace_Concept_With_Inherited_Constraint_109.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/ADT/StringRef.h"

// ========================================================================================================
#define MUT109_OUTPUT 1

void MutatorFrontendAction_109::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef(MT->getSourceRange().getBegin().printToString(Result.Context->getSourceManager())).str();
      //Perform mutation on the source code text by applying string replacement
      std::string newDeclaration = declaration;
      size_t pos = newDeclaration.find("convertible_to");
      if (pos != std::string::npos) {
        newDeclaration.replace(pos, 13, "std::same_as");
      } else {
        pos = newDeclaration.find("convertible_to");
        if (pos != std::string::npos) {
          newDeclaration.replace(pos, 13, "std::derived_from");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), newDeclaration);
    }
}
  
void MutatorFrontendAction_109::MutatorASTConsumer_109::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}