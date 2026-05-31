//source file
#include "../include/Lambda_Return_Type_Mutation_123.h"

// ========================================================================================================
#define MUT123_OUTPUT 1

void MutatorFrontendAction_123::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    if (DL->hasExplicitParameters() == false)
      return;
    auto DLtype = DL->getType();
    auto DLtype_str = DLtype.getAsString();
    if (DLtype_str.find("auto") == string::npos)
      return;
    if (DLtype_str.find("->") == string::npos)
      return;
    llvm::outs() << DLtype_str << '\n';
    auto DL_range = DL->getSourceRange();
    auto DL_str = stringutils::rangetoStr(*(Result.SourceManager), DL_range);
    llvm::outs() << DL_str << '\n';
    auto DL_body = DL->getBody();
    auto DL_body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                               DL_body->getSourceRange());
    llvm::outs() << DL_body_str << '\n';

    auto DL_return_type = DLtype_str.substr(DLtype_str.find("->") + 2);
    llvm::outs() << DL_return_type << '\n';

    auto DL_return_type_str = DL_return_type;
    DL_return_type_str.erase(
        std::remove(DL_return_type_str.begin(), DL_return_type_str.end(), ' '),
        DL_return_type_str.end());
    llvm::outs() << DL_return_type_str << '\n';

    if (DL_return_type_str == "int") {
      DL_str = DL_str.replace(DL_str.find("-> int"), 6, "-> decltype(1+1)");
    } else if (DL_return_type_str == "double") {
      DL_str = DL_str.replace(DL_str.find("-> double"), 9, "-> decltype(1.0)");
    } else if (DL_return_type_str == "float") {
      DL_str = DL_str.replace(DL_str.find("-> float"), 8, "-> decltype(1.0f)");
    } else if (DL_return_type_str == "char") {
      DL_str = DL_str.replace(DL_str.find("-> char"), 7, "-> decltype('a')");
    } else if (DL_return_type_str == "bool") {
      DL_str = DL_str.replace(DL_str.find("-> bool"), 7, "-> decltype(true)");
    } else {
      DL_str = DL_str.replace(DL_str.find("->"), 2, "-> auto");
    }
    DL_str = "/*mut123*/" + DL_str;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL_range), DL_str);
  }
}

void MutatorFrontendAction_123::MutatorASTConsumer_123::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}