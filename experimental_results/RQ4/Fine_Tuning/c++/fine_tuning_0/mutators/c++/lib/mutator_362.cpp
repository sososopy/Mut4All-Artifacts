//source file
#include "../include/Mutator_362.h"

// ========================================================================================================
#define MUT362_OUTPUT 1

void MutatorFrontendAction_362::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isCompleteDefinition() == false)
        return;
      //Get the source code text of target node
      auto decls = MT->decls();
      CXXMethodDecl *deleted_method = nullptr;
      CXXMethodDecl *templated_method = nullptr;
      for (auto decl : decls) {
        if (decl->getKind() == Decl::Kind::CXXMethod) {
          auto method = dyn_cast<CXXMethodDecl>(decl);
          if (method->isDeleted())
            deleted_method = method;
          if (method->getDescribedFunctionTemplate() != nullptr)
            templated_method = method;
        }
      }
      assert(deleted_method != nullptr &&
             "Mut362: The class should have a deleted method!");
      assert(templated_method != nullptr &&
             "Mut362: The class should have a templated method!");
      auto deleted_method_name = deleted_method->getNameAsString();
      auto templated_method_name = templated_method->getNameAsString();
      llvm::outs() << deleted_method_name << ' ' << templated_method_name
                   << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             templated_method->getSourceRange());
      auto class_name = MT->getNameAsString();
      auto ins =
          class_name + " mut362_obj;\nmut362_obj." + deleted_method_name + "();";
      content.insert(content.rfind('}'), ins);
      llvm::outs() << content << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                              templated_method->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_362::MutatorASTConsumer_362::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}