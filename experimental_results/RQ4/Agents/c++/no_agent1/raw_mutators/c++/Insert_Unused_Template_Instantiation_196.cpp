//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unused_Template_Instantiation_196
 */ 
class MutatorFrontendAction_196 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(196)

private:
    class MutatorASTConsumer_196 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_196(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl *> functions;
    };
};

//source file
#include "../include/insert_unused_template_instantiation_196.h"

// ========================================================================================================
#define MUT196_OUTPUT 1

void MutatorFrontendAction_196::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody() || FD->isTemplateInstantiation())
            return;
        
        functions.push_back(FD);
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;
        
        if (!functions.empty()) {
            auto function = functions.back();
            functions.pop_back();
            
            std::string templateDecl = "template<typename T> struct UnusedTemplate { void dummy() {} };";
            std::string instantiation = "UnusedTemplate<int> unusedInstance;";
            
            std::string classContent = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
            size_t insertPos = classContent.rfind('}');
            if (insertPos != std::string::npos) {
                classContent.insert(insertPos, "\n/*mut196*/" + templateDecl + "\n" + instantiation + "\n");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), classContent);
            }
        }
    }
}

void MutatorFrontendAction_196::MutatorASTConsumer_196::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl().bind("Functions");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}