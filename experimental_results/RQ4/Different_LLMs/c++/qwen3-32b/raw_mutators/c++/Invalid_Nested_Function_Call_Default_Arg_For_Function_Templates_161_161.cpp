//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Nested_Function_Call_Default_Arg_For_Function_Templates_161
 */ 
class MutatorFrontendAction_161 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(161)

private:
    class MutatorASTConsumer_161 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_161(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), hExists(false), hDeclared(false) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        bool hExists;
        bool hDeclared;
    };
};

//source file
#include "../include/Invalid_Nested_Function_Call_Default_Arg_For_Function_Templates_161.h"

// ========================================================================================================
#define MUT161_OUTPUT 1

void MutatorFrontendAction_161::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *PV = Result.Nodes.getNodeAs<clang::ParmVarDecl>("defaultArg")) {
        if (!PV || !Result.Context->getSourceManager().isWrittenInMainFile(
                       PV->getLocation()))
            return;

        std::string paramName = PV->getNameAsString();
        std::string newArg = "h(" + paramName + ")()";
        SourceRange paramRange = PV->getSourceRange();
        std::string paramText = stringutils::rangetoStr(*Result.SourceManager, paramRange);
        size_t equalsPos = paramText.find('=');
        if (equalsPos == std::string::npos) return;
        std::string newParamText = paramText.substr(0, equalsPos + 1) + newArg;
        Rewrite.ReplaceText(paramRange, newParamText);

        if (!hExists && !hDeclared) {
            SourceLocation insertionLoc = Result.SourceManager->getLocForStartOfFile(Result.SourceManager->getMainFileID());
            Rewrite.InsertText(insertionLoc, "void h(int);\n", true, true);
            hDeclared = true;
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("hFunc")) {
        hExists = true;
    }
}
  
void MutatorFrontendAction_161::MutatorASTConsumer_161::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        (functionDecl(isTemplateInstantiation(), hasAnyParameter(parmVarDecl(hasDefaultArgument(), hasType(isInteger())).bind("defaultArg"))).bind("funcParam")) || 
        (cxxMethodDecl(isTemplateInstantiation(), hasAnyParameter(parmVarDecl(hasDefaultArgument(), hasType(isInteger())).bind("defaultArg"))).bind("funcParam")) ||
        functionDecl(hasName("h"), hasParameter(0, hasType(isInteger())).bind("hFunc"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}