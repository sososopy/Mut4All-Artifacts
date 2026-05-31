//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Param_With_Array_Struct_98
 */ 
class MutatorFrontendAction_98 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(98)

private:
    class MutatorASTConsumer_98 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_98(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Template_Param_With_Array_Struct_98.h"

// ========================================================================================================
#define MUT98_OUTPUT 1

void MutatorFrontendAction_98::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateDecl")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        // Generate struct code
        std::string structCode = R"(
template<size_t N>
struct arr {
    constexpr size_t size() const { return N; }
    template<typename... Args>
    arr(Args&&... args) {}
};

template<size_t N>
arr(const int(&)[N]) -> arr<N>;
)";
        // Insert struct code at the beginning of the file
        SourceManager &SM = Rewrite.getSourceMgr();
        SourceLocation startLoc = FTD->getBeginLoc();
        FileID fileID = SM.getFileID(startLoc);
        SourceLocation fileStart = SM.getLocForStartOfFile(fileID);
        if (fileStart.isValid()) {
            Rewrite.InsertText(fileStart, structCode, true, true);
        }

        // Modify the function template's template parameters
        TemplateParameterList *TPL = FTD->getTemplateParameters();
        if (!TPL || TPL->size() < 1)
            return;

        // Get the source range of the first template parameter
        TemplateTypeParmDecl *TTP = dyn_cast<TemplateTypeParmDecl>(TPL->getParam(0));
        if (!TTP)
            return;

        SourceRange tplParamRange = TTP->getSourceRange();
        std::string newTplParam = "arr A";
        Rewrite.ReplaceText(tplParamRange, newTplParam);

        // Modify the function parameters
        FunctionDecl *FD = FTD->getTemplatedDecl();
        if (!FD || FD->getNumParams() < 1)
            return;

        ParmVarDecl *firstParam = FD->getParamDecl(0);
        SourceRange paramRange = firstParam->getSourceRange();
        Rewrite.RemoveText(paramRange);
    }
}
  
void MutatorFrontendAction_98::MutatorASTConsumer_98::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasTemplateParameter(0, templateTypeParm())).bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}