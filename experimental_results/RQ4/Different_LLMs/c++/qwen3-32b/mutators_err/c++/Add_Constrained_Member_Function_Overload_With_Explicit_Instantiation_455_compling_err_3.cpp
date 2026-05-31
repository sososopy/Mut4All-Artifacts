//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Constrained_Member_Function_Overload_With_Explicit_Instantiation_455
 */ 
class MutatorFrontendAction_455 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(455)

private:
    class MutatorASTConsumer_455 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_455(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Constrained_Member_Function_Overload_With_Explicit_Instantiation_455.h"

// ========================================================================================================
#define MUT455_OUTPUT 1

void MutatorFrontendAction_455::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *recordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classTemplate")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;
        if (!recordDecl->hasDefinition())
            return;
        const TemplateParameterList *TPL = recordDecl->getTemplateParameterList();
        if (!TPL || TPL->size() == 0)
            return;
        const TemplateTypeParmDecl *TTP = TPL->getParam(0)->getAs<TemplateTypeParmDecl>();
        if (!TTP || TTP->getName().empty())
            return;
        std::string paramName = TTP->getName().str();
        std::string className = recordDecl->getNameAsString();
        std::string conceptName = className + "_C";
        std::string conceptDecl = "template<typename " + paramName + ">\nconcept " + conceptName + " = __is_class(" + paramName + ");\n";
        SourceLocation loc = recordDecl->getLocation();
        Rewrite.InsertTextBefore(loc, conceptDecl);
        if (recordDecl->methods().empty())
            return;
        const CXXMethodDecl *method = *recordDecl->methods().begin();
        if (method->getKind() == Decl::CXXConstructor || method->getKind() == Decl::CXXDestructor)
            return;
        std::string methodName = method->getNameAsString();
        auto methodStr = stringutils::rangetoStr(*Result.SourceManager, method->getSourceRange());
        size_t openBracePos = methodStr.find("{");
        if (openBracePos == std::string::npos)
            return;
        std::string newMethodDecl = methodStr.substr(0, openBracePos) + " requires " + conceptName + "<" + paramName + "> { }";
        SourceLocation classEndLoc = recordDecl->getEndLoc();
        std::string insertionText = "\n    " + newMethodDecl + ";";
        Rewrite.InsertText(classEndLoc, insertionText, true, true);
        std::string explicitInstantiation = "template void " + className + "<int>::" + methodName + ";";
        Rewrite.InsertTextAfter(classEndLoc, "\n" + explicitInstantiation);
    }
}

void MutatorFrontendAction_455::MutatorASTConsumer_455::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition(), has(templateParameters())).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}