//source file
#include "../include/Nested_Template_Constrained_NTTP_94.h"

// ========================================================================================================
#define MUT94_OUTPUT 1

void MutatorFrontendAction_94::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("OuterTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;
        
        const CXXRecordDecl* outerRecord = CT->getTemplatedDecl();
        if (!outerRecord || !outerRecord->isCompleteDefinition())
            return;
        
        outerTemplates.push_back(outerRecord);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithStaticAssert")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (outerTemplates.empty())
            return;
        
        size_t idx = getrandom::getRandomIndex(outerTemplates.size() - 1);
        const CXXRecordDecl* outerRecord = outerTemplates[idx];
        
        string conceptName = "AlwaysTrue";
        string innerTemplateName = "InnerTemplate";
        string nttpType = "int";
        string nttpValue = "0";
        
        string conceptDecl = "\ntemplate <class T> concept " + conceptName + " = true;\n";
        
        string innerTemplateDecl = "\ntemplate <class U, " + conceptName + " auto N>\nstruct " + innerTemplateName + " {\n    static const int value = 0;\n};";
        
        string outerTemplateName = outerRecord->getNameAsString();
        string staticAssertText = "\nstatic_assert(" + outerTemplateName + "<void>::" + innerTemplateName + "<" + nttpType + ", " + nttpValue + ">::value == 0);";
        
        SourceLocation insertLoc = FD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, conceptDecl + innerTemplateDecl + staticAssertText);
    }
}

void MutatorFrontendAction_94::MutatorASTConsumer_94::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher outerMatcher = classTemplateDecl().bind("OuterTemplate");
    DeclarationMatcher functionMatcher = functionDecl().bind("FunctionWithStaticAssert");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(outerMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}