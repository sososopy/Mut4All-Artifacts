//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Alias_Template_Expansion_2
 */ 
class MutatorFrontendAction_2 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(2)
private:
    class MutatorASTConsumer_2 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_2(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Alias_Template_Expansion_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("AliasTemplate")) {
        //Filter nodes in header files
        if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getTypeLoc()))
            return;

        //Get the source code text of target node
        auto sourceRange = AT->getTypeLoc();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        //Check if the alias template expansion uses only a parameter pack
        auto templateArgs = AT->template_arguments();
        if (templateArgs.size() == 0)
            return;

        //Look for an extra template argument in the current scope
        auto *contextDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("ContextFunction");
        if (!contextDecl)
             return;

        //Find a template parameter from the context (member function or class)
        auto *templateParam = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("ExtraTemplateParam");
        if (!templateParam)
            return;

        //Perform mutation on the source code text by applying string replacement
        auto extraArgName = templateParam->getNameAsString();
        if (sourceText.find("...") != std::string::npos) {
            //Append the extra argument to the pack
            size_t packEnd = sourceText.find(">");
            if (packEnd != std::string::npos) {
                sourceText.insert(packEnd - 1, ", " + extraArgName);
            }
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), "/*mut2*/" + sourceText);
    }
}

void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateSpecializationType(
        hasTemplateName(hasDeclaration(templateDecl().bind("AliasDecl"))),
        hasParent(functionDecl().bind("ContextFunction"))
    ).bind("AliasTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}