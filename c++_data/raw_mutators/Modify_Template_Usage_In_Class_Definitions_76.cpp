//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Usage_In_Class_Definitions_76
 */ 
class MutatorFrontendAction_76 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(76)

private:
    class MutatorASTConsumer_76 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_76(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> templateClasses;
    };
};

//source file
#include "../include/modify_template_usage_in_class_definitions_76.h"

// ========================================================================================================
#define MUT76_OUTPUT 1

void MutatorFrontendAction_76::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (!TD->isCompleteDefinition())
            return;

        // Get the source code text of target node
        auto classContent = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
        
        // Perform mutation on the source code text by applying string replacement
        std::string constevalFunction = "\n\tconsteval void mutatedFunction(T... values) {}\n";
        if (classContent.rfind('}') != std::string::npos)
            classContent.insert(classContent.rfind('}'), constevalFunction);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), classContent);
        templateClasses.push_back(TD);
    }
}

void MutatorFrontendAction_76::MutatorASTConsumer_76::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl())).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}