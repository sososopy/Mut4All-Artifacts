//source file
#include "../include/Mutator_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TU = Result.Nodes.getNodeAs<clang::TypedefDecl>("TransparentUnion")) {
      //Filter nodes in header files
      if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
        return;

      if (const auto *UD = TU->getUnderlyingType()->getAs<clang::RecordType>()) {
        if (const auto *UnionDecl = llvm::dyn_cast<clang::CXXRecordDecl>(UD->getDecl())) {
          //Get the source code text of target node
          auto unionSource = stringutils::rangetoStr(*(Result.SourceManager), UnionDecl->getSourceRange());
          //Perform mutation on the source code text by applying string replacement
          std::string forwardDecl = "union ForwardDeclared;";
          auto insertPos = unionSource.find_last_of('}');
          if (insertPos != std::string::npos) {
            unionSource.insert(insertPos, forwardDecl);
          }
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(UnionDecl->getSourceRange()), unionSource);
        }
      }
    }
}
  
void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typedefDecl(hasType(recordType(hasDeclaration(cxxRecordDecl(isUnion())))), hasAttr(clang::attr::TransparentUnion)).bind("TransparentUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}