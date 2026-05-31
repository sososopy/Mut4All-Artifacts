//source file
#include "../include/Add_Nested_Template_Class_With_Lambda_463.h"

// ========================================================================================================
#define MUT463_OUTPUT 1

void MutatorFrontendAction_463::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        if (!MT->hasBody())
            return;
        
        const CompoundStmt *Body = clang::cast<CompoundStmt>(MT->getBody());
        if (!Body)
            return;
        
        SourceRange BodyRange = Body->getSourceRange();
        if (!BodyRange.isValid())
            return;
        
        const ParmVarDecl *FirstParam = MT->getParamDecl(0);
        if (!FirstParam || FirstParam->getName().empty())
            return;
        
        std::string ParamName = FirstParam->getName().str();
        std::string NewStruct = R"(
  struct TemplateWrapper {
    template<typename U> auto operator()(U param) {
      auto lambda = [" + ParamName + R"](auto x) { return x.Method(); };
      return lambda(param);
    }
  };
  TemplateWrapper{}(" + ParamName + R");
)";
        
        SourceManager &SM = *Result.SourceManager;
        std::string OriginalBody = stringutils::rangetoStr(SM, BodyRange);
        size_t OpenBracePos = OriginalBody.find("{");
        if (OpenBracePos == std::string::npos)
            return;
        
        std::string MutatedBody = OriginalBody;
        MutatedBody.insert(OpenBracePos + 1, NewStruct);
        
        Rewrite.ReplaceText(BodyRange, MutatedBody);
    }
}

void MutatorFrontendAction_463::MutatorASTConsumer_463::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(hasBody(compoundStmt())).bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}