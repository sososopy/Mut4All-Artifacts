//source file
#include "../include/Templated_Lambda_Usage_436.h"

// ========================================================================================================
#define MUT436_OUTPUT 1

void MutatorFrontendAction_436::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isCompleteDefinition() == false)
      return;
    //Get the source code text of target node
    auto name = DL->getNameAsString();
    //Perform mutation on the source code text by applying string replacement
    llvm::outs() << name << '\n';
    type_names.push_back(name);
  } else if (auto *TL = Result.Nodes.getNodeAs<clang::LambdaExpr>("TPLambda")) {
    if (!TL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   TL->getBeginLoc()))
      return;
    //Get the source code text of target node
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), TL->getSourceRange());
    auto template_param = TL->getTemplateParameterList();
    //llvm::outs() << template_param->getNameAsString() << '\n';
    auto param_name = stringutils::rangetoStr(*(Result.SourceManager), template_param->getSourceRange());
    std::string replacement;
    if (type_names.size() == 0) {
      replacement = "int";
    } else {
      size_t index = getrandom::getRandomIndex(type_names.size() - 1);
      replacement = type_names[index];
    }
    //Perform mutation on the source code text by applying string replacement
    //Replace the original AST node with the mutated one
    llvm::outs() << replacement << '\n';
    llvm::outs() << content << '\n';
    stringutils::strReplace(content, param_name, replacement);
    llvm::outs() << content << '\n';
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TL->getSourceRange()), content);
  }
}
  
void MutatorFrontendAction_436::MutatorASTConsumer_436::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto tplambda_matcher = lambdaExpr(has(cxxRecordDecl())).bind("TPLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(tplambda_matcher, &callback);
    matchFinder.matchAST(Context);
}