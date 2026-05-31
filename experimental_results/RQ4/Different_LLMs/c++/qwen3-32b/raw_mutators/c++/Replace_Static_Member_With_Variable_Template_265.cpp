//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Static_Member_With_Variable_Template_265
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
#include "../include/Replace_Static_Member_With_Variable_Template_265.h"

// ========================================================================================================
#define MUT265_OUTPUT 1

void MutatorFrontendAction_265::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *structDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructTemplate")) {
        if (!structDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                           structDecl->getLocation()))
            return;

        if (auto *tmpl = structDecl->getDescribedClassTemplate()) {
            auto params = tmpl->getTemplateParameters();
            if (!params || params->size() < 1) {
                return;
            }
        } else {
            return;
        }

        SourceLocation endLoc = structDecl->getEndLoc();
        if (endLoc.isInvalid()) {
            return;
        }

        std::string varTemplate = R"(
    template <typename U>
    static constexpr bool value = false;
)";
        varTemplate = varTemplate.substr(1);
        Rewrite.InsertText(endLoc, varTemplate, true, true);

        std::string structName = structDecl->getNameAsString();
        std::string partialSpec = R"(
template <template <typename...> class Container, typename... Params>
constexpr bool )";
        partialSpec += structName + "<Container<Params...>>::value<Container> = true;";
        Rewrite.InsertTextAfterToken(endLoc, partialSpec);
    }
}

void MutatorFrontendAction_265::MutatorASTConsumer_265::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isTemplated()).bind("StructTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}