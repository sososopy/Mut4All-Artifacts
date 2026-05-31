//source file
#include "../include/replace_variable_with_constexpr_413.h"

// ========================================================================================================
#define MUT413_OUTPUT 1

void MutatorFrontendAction_413::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("Variable")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                   VD->getLocation()))
            return;

        if (VD->isLocalVarDecl() && VD->hasInit()) {
            auto varName = VD->getNameAsString();
            auto initExpr = VD->getInit()->IgnoreImpCasts();
            std::string initValue;
            llvm::raw_string_ostream rso(initValue);
            initExpr->printPretty(rso, nullptr, Result.Context->getPrintingPolicy());

            std::string constexprDecl = "constexpr auto " + varName + " = " + initValue + ";";
            variableToConstexprMap[varName] = constexprDecl;

            SourceLocation startLoc = VD->getBeginLoc();
            SourceLocation endLoc = VD->getEndLoc().getLocWithOffset(1);
            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), "/*mut413*/" + constexprDecl);
        }
    } else if (auto *Ref = Result.Nodes.getNodeAs<clang::DeclRefExpr>("VarRef")) {
        if (!Ref || !Result.Context->getSourceManager().isWrittenInMainFile(
                   Ref->getLocation()))
            return;

        auto varName = Ref->getNameInfo().getAsString();
        if (variableToConstexprMap.find(varName) != variableToConstexprMap.end()) {
            Rewrite.ReplaceText(Ref->getSourceRange(), "/*mut413*/" + varName);
        }
    }
}

void MutatorFrontendAction_413::MutatorASTConsumer_413::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto varMatcher = varDecl(hasInitializer(anything())).bind("Variable");
    auto refMatcher = declRefExpr(to(varDecl())).bind("VarRef");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(refMatcher, &callback);
    matchFinder.matchAST(Context);
}