//source file
#include "../include/Inline_Function_Expansion_In_Template_Instantiation_408.h"

// ========================================================================================================
#define MUT408_OUTPUT 1

void MutatorFrontendAction_408::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (!FD->hasBody())
            return;
        
        Stmt *Body = FD->getBody();
        if (!Body)
            return;
        
        SourceManager &SM = Result.Context->getSourceManager();
        if (!SM.isInMainFile(Body->getBeginLoc()))
            return;
        
        std::string BodyText = stringutils::rangetoStr(SM, Body->getSourceRange());
        if (FD->getNumParams() > 0) {
            const ParmVarDecl *Param = FD->getParamDecl(0);
            QualType ParamType = Param->getType();
            if (ParamType->isIntegerType()) {
                std::string ParamName = Param->getNameAsString();
                std::string ParamTypeStr = ParamType.getAsString();
                std::string FuncName = FD->getNameAsString();
                
                std::string InsertCode = ParamTypeStr + " y = " + ParamName + " + 1;\n    " + FuncName + "(y);\n";
                
                size_t OpenBracePos = BodyText.find('{');
                if (OpenBracePos != std::string::npos) {
                    BodyText.insert(OpenBracePos + 1, InsertCode);
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(Body->getSourceRange()), BodyText);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_408::MutatorASTConsumer_408::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        isInstantiated(),
        anyOf(isInline(), hasAttr(attr::AlwaysInline)),
        hasBody(compoundStmt())
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}