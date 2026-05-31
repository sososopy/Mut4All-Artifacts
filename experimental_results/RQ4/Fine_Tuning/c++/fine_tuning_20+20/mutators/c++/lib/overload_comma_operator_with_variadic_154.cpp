//source file
#include "../include/Overload_Comma_Operator_With_Variadic_154.h"

// ========================================================================================================
#define MUT154_OUTPUT 1

void MutatorFrontendAction_154::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      //Record the candidate existing class names for insertion
      cur_classes.push_back(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Main")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getNameAsString() != "main")
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("operator,") != string::npos)
        return;
      if (content.find("ClassX") != string::npos) {
        content.insert(content.rfind('}'), "\n/*mut154*/auto mut_154=(ClassX(),\"test\");\n");
        llvm::outs() << content << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(DL->getSourceRange()), content);
        return;
      }
      for (std::size_t j = 0; j < cur_classes.size(); ++j) {
        if (cur_classes[j]->getNameAsString() == "ClassX") {
          content.insert(content.rfind('}'), "\n/*mut154*/auto mut_154=(ClassX(),\"test\");\n");
          llvm::outs() << content << '\n';
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(DL->getSourceRange()), content);
          return;
        }
      }
      content.insert(content.rfind('}'), "\n/*mut154*/auto mut_154=(ClassX(),\"test\");\n");
      content = "class ClassX{};\n" + content;
      llvm::outs() << content << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}

void MutatorFrontendAction_154::MutatorASTConsumer_154::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    auto matcher = cxxRecordDecl().bind("Classes");
    auto main_matcher = functionDecl().bind("Main");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(main_matcher, &callback);
    matchFinder.matchAST(Context);
}