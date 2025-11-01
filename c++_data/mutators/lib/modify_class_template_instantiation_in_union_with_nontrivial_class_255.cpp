//source file
#include "../include/Modify_Class_Template_Instantiation_In_Union_With_Nontrivial_Class_255.h"

// ========================================================================================================
#define MUT255_OUTPUT 1

void MutatorFrontendAction_255::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("UnionTemplate")) {
        if (!UT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       UT->getLocation()))
            return;
        
        if (!UT->isUnion())
            return;

        for (auto *Field : UT->fields()) {
            if (auto *TST = Field->getType()->getAs<clang::TemplateSpecializationType>()) {
                const clang::TemplateDecl *TemplateDecl = TST->getTemplateName().getAsTemplateDecl();
                if (TemplateDecl) {
                    // Modify the template argument to a more complex type
                    std::string newType = "MyType";
                    std::string oldType = TST->getArg(0).getAsType().getAsString();
                    
                    std::string newCode = "MyClass<" + newType + "> instance;";
                    std::string oldCode = "MyClass<" + oldType + "> instance;";
                    
                    clang::SourceRange range = Field->getSourceRange();
                    clang::SourceLocation start = range.getBegin();
                    clang::SourceLocation end = range.getEnd();
                    
                    Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(start, end), newCode);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_255::MutatorASTConsumer_255::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isUnion()).bind("UnionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}