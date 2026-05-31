//header file
#pragma once
#include "Mutator_base.h"

/**
 * CTAD_Alias_Template_Chain_53
 */ 
class MutatorFrontendAction_53 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(53)

private:
    class MutatorASTConsumer_53 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_53(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), hasStructSLoc(false), hasStructSName(false), hasAliasProxyLoc(false), hasFunctionLoc(false) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        SourceLocation structSLoc;
        bool hasStructSLoc;
        std::string structSName;
        bool hasStructSName;
        SourceLocation aliasProxyLoc;
        bool hasAliasProxyLoc;
        SourceLocation functionLoc;
        bool hasFunctionLoc;
    };
};

//source file
#include "../include/CTAD_Alias_Template_Chain_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("structS")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;
        structSLoc = TD->getLocation();
        hasStructSLoc = true;
        structSName = TD->getNameAsString();
        hasStructSName = true;
    } else if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("aliasProxy")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;
        aliasProxyLoc = UD->getLocation();
        hasAliasProxyLoc = true;
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        functionLoc = FD->getLocation();
        hasFunctionLoc = true;
    }

    if (hasStructSLoc && hasAliasProxyLoc && hasFunctionLoc && hasStructSName) {
        std::string newAlias = "template<typename T>\nusing C = Proxy<" + structSName + "<T>>;\n";
        Rewrite.InsertTextAfter(aliasProxyLoc, newAlias);

        if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("function")) {
            if (FD->hasBody()) {
                const auto *body = FD->getBody();
                if (body) {
                    SourceLocation insertLoc = body->getRBraceLoc();
                    std::string varDecl = "C obj{ 0 };";
                    Rewrite.InsertTextBefore(insertLoc, varDecl);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher structMatcher = classTemplateDecl(
        has(cxxRecordDecl(
            has(cxxConstructorDecl(
                parameterCountIs(1),
                hasParameter(0, hasType(templateTypeParm(0))) ))))
    ).bind("structS");
    DeclarationMatcher aliasMatcher = usingDecl(
        isTemplate(),
        has(numTemplateParameters(1)),
        hasType(typeParmType())
    ).bind("aliasProxy");
    DeclarationMatcher functionMatcher = functionDecl(hasName("main")).bind("function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(aliasMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}