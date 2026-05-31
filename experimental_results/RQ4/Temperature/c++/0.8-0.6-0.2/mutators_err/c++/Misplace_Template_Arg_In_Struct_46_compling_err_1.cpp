//header file
#pragma once
#include "Mutator_base.h"

/**
 * misplace_template_arg_in_struct_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/misplace_template_arg_in_struct_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("StaticFunc")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
            return;
        
        // Get the return type
        QualType returnType = MT->getReturnType();
        if (const TemplateSpecializationType *TST = returnType->getAs<TemplateSpecializationType>()) {
            // Get the source code text of target node
            SourceRange range = TST->getSourceRange();
            std::string replacement = TheRewriter.getRewrittenText(range);

            // Perform mutation on the source code text by applying string replacement
            if (replacement.find(">>") == std::string::npos) {
                replacement.insert(replacement.find(">"), ">");
            } else {
                replacement.erase(replacement.find_last_of(">"));
            }

            // Replace the original AST node with the mutated one
            TheRewriter.ReplaceText(range, replacement);
        }
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isStaticStorageClass(), returns(templateSpecializationType())).bind("StaticFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}