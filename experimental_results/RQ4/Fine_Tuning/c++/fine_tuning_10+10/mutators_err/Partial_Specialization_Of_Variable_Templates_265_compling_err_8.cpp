//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * Partial_Specialization_Of_Variable_Templates_265
 */ 
class MutatorFrontendAction_265 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(265)

private:
    class MutatorASTConsumer_265 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_265(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Partial_Specialization_Of_Variable_Templates_265.h"

// ========================================================================================================
#define MUT265_OUTPUT 1

void MutatorFrontendAction_265::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplate")) {
        if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VT->getLocation()))
            return;

        auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                    VT->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string partialSpecialization = R"(
            template <template <typename...> class Container, typename... Args>
            struct Example<Container<Args...>, Container> {
                static constexpr bool value = true;
            };
        )";
        
        if (templateDecl.find("template <typename T, template <typename...> class Container>") != std::string::npos) {
            templateDecl.replace(templateDecl.find("template <typename T, template <typename...> class Container>"), 
                                 std::string("template <typename T, template <typename...> class Container>").length(), 
                                 partialSpecialization);
        }

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VT->getSourceRange()), templateDecl);
    }
}

void MutatorFrontendAction_265::MutatorASTConsumer_265::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varTemplateDecl().bind("VarTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}