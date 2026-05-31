//source file
#include "../include/Expand_Template_Argument_In_Struct_Or_Class_178.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      cur_classes.push_back(CL);
    } else if (auto *TI = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Type")) {
      if (!TI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TI->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), TI->getSourceRange());
      if (content.find("__make_integer_seq") != string::npos) {
        llvm::outs() << content << '\n';
        auto pos = content.rfind('>');
        if (pos != string::npos) {
          size_t index = getrandom::getRandomIndex(cur_classes.size() - 1);
          auto target = cur_classes[index];
          content.insert(pos, ", " + target->getNameAsString());
          llvm::outs() << content << '\n';
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(TI->getSourceRange()), content);
        }
      }
    }
}
  
void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto type_matcher = typeAliasDecl().bind("Type");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(type_matcher, &callback);
    matchFinder.matchAST(Context);
}