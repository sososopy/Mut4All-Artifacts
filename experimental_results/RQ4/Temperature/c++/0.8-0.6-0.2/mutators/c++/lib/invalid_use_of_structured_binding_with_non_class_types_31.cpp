//source file
#include "../include/invalid_use_of_structured_binding_with_non_class_types_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("structuredBinding")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                VD->getLocation()))
            return;
        
        if (auto *DD = VD->getType()->getAs<clang::RecordType>()) {
            auto decl = DD->getDecl();
            if (decl->isStruct() || decl->isClass()) {
                auto originalRange = VD->getSourceRange();
                auto originalText = stringutils::rangetoStr(*(Result.SourceManager), originalRange);
                
                std::string replacementText = "int " + VD->getNameAsString() + " = 42";
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(originalRange), replacementText);
            }
        }
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(recordType())).bind("structuredBinding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}