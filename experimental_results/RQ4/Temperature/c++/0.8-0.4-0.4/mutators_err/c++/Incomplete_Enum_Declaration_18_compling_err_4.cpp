//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_enum_declaration_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "MutatorFrontendAction_18.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<EnumDecl>("EnumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;

        if (ED->isCompleteDefinition() && ED->getNumEnumerators() > 0) {
            auto sourceRange = ED->getSourceRange();
            auto &SM = *Result.SourceManager;
            auto enumText = Lexer::getSourceText(CharSourceRange::getTokenRange(sourceRange), SM, LangOptions());
            
            size_t closingBracePos = enumText.rfind('}');
            if (closingBracePos != std::string::npos) {
                enumText.erase(closingBracePos, 1);  // Remove the closing brace
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), enumText);
            }
        }
    }
}

void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl(hasParent(cxxRecordDecl())).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}