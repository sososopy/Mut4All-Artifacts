//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Template_Parameter_Static_Member_429
 */ 
class MutatorFrontendAction_429 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(429)

private:
    class MutatorASTConsumer_429 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_429(Rewriter &R) : TheRewriter(R) {}
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
        // Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Template_Template_Parameter_Static_Member_429.h"

// ========================================================================================================
#define MUT429_OUTPUT 1

void MutatorFrontendAction_429::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        const auto *classDecl = CT->getTemplatedDecl();
        if (!classDecl || classDecl->getNameAsString().empty())
            return;

        std::string className = classDecl->getNameAsString();
        SourceRange classRange = classDecl->getSourceRange();
        std::string classText = stringutils::rangetoStr(*Result.SourceManager, classRange);

        std::string tParamName = "T";
        const TemplateParameterList *tparams = classDecl->getTemplateParameters();
        for (const auto *param : tparams->asArray()) {
            if (const auto *ttParam = dyn_cast<TemplateTemplateParmDecl>(param)) {
                tParamName = ttParam->getNameAsString();
                break;
            }
        }

        std::string helperCode = R"(
    public:
        template<typename U>
        using Helper = typename )" + tParamName + R"(<U>::type;
        static Helper<int> S;
)";
        size_t pos = classText.rfind('}');
        if (pos != std::string::npos) {
            classText.insert(pos, helperCode);
        }

        Rewrite.ReplaceText(classRange, classText);

        SourceLocation insertLoc = classDecl->getEndLoc();
        std::string accessStmt = "\nint x = " + className + "<void>::S;\n";
        Rewrite.InsertTextAfterToken(insertLoc, accessStmt);
    }
}

void MutatorFrontendAction_429::MutatorASTConsumer_429::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(templateTemplateParameter())).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}