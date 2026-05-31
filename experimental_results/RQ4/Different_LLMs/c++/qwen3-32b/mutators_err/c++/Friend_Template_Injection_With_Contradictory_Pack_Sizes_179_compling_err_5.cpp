//header file
#pragma once
#include "Mutator_base.h"

/**
 * Friend_Template_Injection_With_Contradictory_Pack_Sizes_179
 */ 
class MutatorFrontendAction_179 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(179)

private:
    class MutatorASTConsumer_179 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_179(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Friend_Template_Injection_With_Contradictory_Pack_Sizes_179.h"

// ========================================================================================================
#define MUT179_OUTPUT 1

void MutatorFrontendAction_179::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classTemplate")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        std::string classText = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());

        std::string friendFunc = R"(
            template<class... Args>
            friend void helper(int, type_t<Args...>) {
                static_assert(sizeof...(Args) != 0, "Assertion 1");
                static_assert(sizeof...(Args) == 0, "Assertion 2");
                constexpr auto n = sizeof...(Args);
                nothing<n>();
            }
        )";

        std::string::size_type pos = classText.rfind('}');
        if (pos != std::string::npos) {
            classText.insert(pos, friendFunc);
            Rewrite.ReplaceText(MT->getSourceRange(), classText);
        }
    }
}

void MutatorFrontendAction_179::MutatorASTConsumer_179::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(clang::ast_matchers::isClassTemplate()).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}