//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Struct_Default_NTTP_129
 */ 
class MutatorFrontendAction_129 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(129)

private:
    class MutatorASTConsumer_129 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_129(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Struct_Default_NTTP_129.h"

// ========================================================================================================
#define MUT129_OUTPUT 1

void MutatorFrontendAction_129::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateStruct")) {
        //Filter nodes in header files
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
            return;

        //Get the source code text of target node
        auto templateDecl = stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        size_t pos = templateDecl.find("auto =");
        if (pos != std::string::npos) {
            templateDecl.replace(pos, std::string("auto =").length(), "auto = 42");
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), templateDecl);
    }
}
  
void MutatorFrontendAction_129::MutatorASTConsumer_129::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(isStruct()))).bind("TemplateStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}