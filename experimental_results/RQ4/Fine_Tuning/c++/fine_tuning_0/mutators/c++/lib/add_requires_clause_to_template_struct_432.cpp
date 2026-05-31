//source file
#include "../include/Add_Requires_Clause_To_Template_Struct_432.h"

// ========================================================================================================
#define MUT432_OUTPUT 1

void MutatorFrontendAction_432::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("StructTemplates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getName();
      auto decls = MT->getTemplateParameters();
      auto decls_str = stringutils::rangetoStr(*(Result.SourceManager),
                                               decls->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      auto mut_str = "/*mut432*/requires true ";
      llvm::outs() << decls_str << '\n';
      decls_str = decls_str + " " + mut_str;
      llvm::outs() << decls_str << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(decls->getSourceRange(), decls_str);
    }
}
  
void MutatorFrontendAction_432::MutatorASTConsumer_432::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(isStruct()))).bind("StructTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}