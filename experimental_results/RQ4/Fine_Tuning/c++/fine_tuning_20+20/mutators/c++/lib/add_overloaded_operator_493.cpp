//source file
#include "../include/Add_Overloaded_Operator_493.h"

// ========================================================================================================
#define MUT493_OUTPUT 1

void MutatorFrontendAction_493::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      cur_classes.push_back(CL);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "TargetClass")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      if ((!TD->isStruct() && !TD->isClass()) || TD->isLambda())
        return;
      if (!TD->isCompleteDefinition())
        return;
      if (TD->isStruct())
        return;
      auto methods = TD->methods();
      string overload_func = "";
      if (TD->isStruct()) {
        overload_func += "/*mut493*/\n";
        overload_func += "bool operator==(const ";
        overload_func += TD->getNameAsString();
        overload_func += "&rhs) const {\n";
        overload_func += "return true;\n}\n";
        llvm::outs() << overload_func;
        Rewrite.ReplaceText(TD->getEndLoc(), 0, overload_func);
        return;
      }
      if (methods.empty()) {
        overload_func += "/*mut493*/\n";
        overload_func += "bool operator==(const ";
        overload_func += TD->getNameAsString();
        overload_func += "&rhs) const {\n";
        overload_func += "return true;\n}\n";
        llvm::outs() << overload_func;
        Rewrite.ReplaceText(TD->getEndLoc(), 0, overload_func);
        return;
      }
      overload_func += "/*mut493*/\n";
      overload_func += "bool operator==(const ";
      overload_func += TD->getNameAsString();
      overload_func += "&rhs) const {\n";
      auto fields = TD->fields();
      if (fields.empty()) {
        overload_func += "return true;\n";
      } else {
        overload_func += "return ";
        for (auto field : fields) {
          overload_func += field->getNameAsString();
          overload_func += "==rhs.";
          overload_func += field->getNameAsString();
          overload_func += "&&";
        }
        overload_func += "true;\n";
      }
      overload_func += "}\n";
      llvm::outs() << overload_func;
      Rewrite.ReplaceText(TD->getEndLoc(), 0, overload_func);
    }
  }
  
void MutatorFrontendAction_493::MutatorASTConsumer_493::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto target_matcher = cxxRecordDecl(has(cxxMethodDecl())).bind("TargetClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(target_matcher, &callback);
    matchFinder.matchAST(Context);
}