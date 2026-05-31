//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

/**
 * incomplete_enum_declaration_18
 */ 
class MutatorFrontendAction_18 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(18)

private:
    class MutatorASTConsumer_18 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_18(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
    private:
        clang::Rewriter &Rewrite;
    };
};

//source file
#include "MutatorFrontendAction_18.h"

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